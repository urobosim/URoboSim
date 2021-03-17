
#include "Controller/RJointController.h"

URJointController::URJointController()
{
  BaseLink = TEXT("base_footprint");
  State = UJointControllerState::Normal;
  Mode = UJointControllerMode::Dynamic;
}

void URJointController::SetJointNames(TArray<FString> InNames)
{
  TrajectoryStatus.JointNames = InNames;
  int JointNum = InNames.Num();
  TrajectoryStatus.Error.Empty();
  TrajectoryStatus.Error.AddDefaulted(JointNum);
  TrajectoryStatus.Desired.Empty();
  TrajectoryStatus.Desired.AddDefaulted(JointNum);
  TrajectoryStatus.Position.Empty();
  TrajectoryStatus.Position.AddDefaulted(JointNum);
  bTrajectoryPointsReached.Empty();
  bTrajectoryPointsReached.AddDefaulted(JointNum);
  ActionDuration = 0.0;
  TrajectoryPointIndex = 0;
  Trajectory.Empty();
}


void URJointController::SetJointVelocities(float InDeltaTime)
{
  if(State == UJointControllerState::FollowJointTrajectory)
    {
      for(int i = 0; i < TrajectoryStatus.JointNames.Num(); i++)
        {
          URJoint* Joint = GetOwner()->Joints[TrajectoryStatus.JointNames[i]];
          if(Joint)
            {
              Joint->SetJointVelocity(Trajectory[TrajectoryPointIndex].Velocities[i]);
            }
        }
    }

}

void URJointController::UpdateDesiredJointAngle(float InDeltaTime)
{
  if(State == UJointControllerState::FollowJointTrajectory)
    {

      float NextTimeStep = Trajectory[TrajectoryPointIndex].GetTimeAsDouble();
      float OldTimeStep = OldTrajectoryPoints.GetTimeAsDouble();
      float CurrentTimeStep = ActionDuration;
      float DiffTrajectoryTimeStep =  NextTimeStep - OldTimeStep;

      //if execution is slower than the trajectory demands, use the NextTimeStep for further calculations
      //if execution is faster than the trajectory demands, use the OldTimeStep for further calculations
      // in order to cap the linear interpolation between the points
      if(ActionDuration < OldTimeStep)
        {
          CurrentTimeStep = OldTimeStep;
        }
      else if(ActionDuration > NextTimeStep)
        {
          CurrentTimeStep = NextTimeStep;
        }

      for(int i = 0; i < TrajectoryStatus.JointNames.Num(); i++)
        {
          FString JointName = TrajectoryStatus.JointNames[i];
          float& JointState = DesiredJointState.FindOrAdd(JointName);
          float DiffJointStep;
          DiffJointStep = Trajectory[TrajectoryPointIndex].Points[i] - OldTrajectoryPoints.Points[i];
          JointState = DiffJointStep / DiffTrajectoryTimeStep * (CurrentTimeStep - OldTimeStep) + OldTrajectoryPoints.Points[i];
        }
    }
}

bool URJointController::CheckTrajectoryPoint()
{
  bool bAllPointsReady = true;
  float NextTimeStep = Trajectory[TrajectoryPointIndex].GetTimeAsDouble();
  if(NextTimeStep == 0)
    {
      TrajectoryPointIndex++;
    }

  for(int i = 0; i < TrajectoryStatus.JointNames.Num(); i++)
    {
      URJoint* Joint = GetOwner()->Joints[TrajectoryStatus.JointNames[i]];
      if(Joint)
        {
          float CurrentJointPos = Joint->GetEncoderValue();
          float DesiredPos = Trajectory[TrajectoryPointIndex].Points[i];
          float Diff = DesiredPos - CurrentJointPos;

          TrajectoryStatus.Position[i] = CurrentJointPos;
          TrajectoryStatus.Desired[i] = DesiredPos;
          TrajectoryStatus.Error[i] = Diff;
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("%s of Trajectory not contained in RobotModel"), *TrajectoryStatus.JointNames[i]);
        }
    }

  GoalStatusList.Last().Status = 1;

  if(ActionDuration > NextTimeStep)
    {
      float CurrentTimeStep = ActionDuration;
      OldTrajectoryPoints = Trajectory[TrajectoryPointIndex];
      TrajectoryPointIndex++;
      return true;
    }

  return false;
}

bool URJointController::CheckTrajectoryGoalReached()
{
  if(TrajectoryPointIndex == Trajectory.Num())
    {
      State = UJointControllerState::Normal;
      bPublishResult = true;

      GoalStatusList.Last().Status = 3;

      Trajectory.Empty();
      TrajectoryPointIndex = 0;
      ActionDuration = 0.0;
      return true;
    }

  return false;
}

void URJointController::CallculateJointVelocities(float InDeltaTime)
{
  FString Velocity = "";
  for(auto & Joint: GetOwner()->Joints)
    {
      if(DesiredJointState.Contains(Joint.Key))
        {
          Joint.Value->bActuate = true;
          float DesiredPos = 0.0f;
          DesiredPos = DesiredJointState[Joint.Key];



          float CurrentJointPos = Joint.Value->GetEncoderValue();
          float Diff = DesiredPos - CurrentJointPos;
          
          
          Diff = Joint.Value->Constraint->CheckPositionRange(Diff);
          if(Joint.Key.Contains("iiwa_joint_4"))
          {
           Diff = -Diff;
          }

          float Vel = Diff / InDeltaTime;
          float VelSave = Vel;
          // if(Joint.Value->MaxJointVel > 0)
          //   {
          //     if(FMath::Abs(Vel) > Joint.Value->MaxJointVel)
          //       {
          //         Vel = Vel / FMath::Abs(Vel) * Joint.Value->MaxJointVel;
          //       }
          //   }
          // if(Joint.Value->GetName().Contains("torso_lift_joint"))
          //   {
          //     // UE_LOG(LogTemp, Error, TEXT("Vel: %f VelSave %f"), Vel, VelSave);
          //     UE_LOG(LogTemp, Error, TEXT("CurrentJointPos: %f DesiredPos %f Diff %f"), CurrentJointPos, DesiredPos, Diff);
          //   }
          Joint.Value->SetJointVelocity(Vel);
        }
    }
}

void URJointController::SetDesiredJointState(FString JointName, float InJointState)
{
  URJoint* Joint = GetOwner()->Joints[JointName];
  if(Joint)
    {
      float& JointValue = DesiredJointState.FindOrAdd(JointName);
      // UE_LOG(LogTemp, Warning, TEXT("JointName %s Upper %f Lower %f"), *JointName, Joint->Constraint->Upper, Joint->Constraint->Lower);
      //TODO: Remove hardcode

      JointValue = Joint->Constraint->ClampJointStateToConstraintLimit(InJointState);

    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Setting DesiredJointState failed. Joint %s not contained in Model"), *JointName);
    }
}

void URJointController::Tick(float InDeltaTime)
{
  if(!GetOwner())
    {
      UE_LOG(LogTemp, Error, TEXT("Model not initialized"));
      return;
    }

  for(auto& Joint : GetOwner()->Joints)
    {
      Joint.Value->UpdateEncoder();
    }

  switch(State)
    {
    case UJointControllerState::FollowJointTrajectory:
      ActionDuration+= InDeltaTime * SpeedFactorHack;
      if(!CheckTrajectoryPoint())
        {
          UpdateDesiredJointAngle(InDeltaTime);
          // SetJointVelocities(InDeltaTime);
        }
      else
        {
          if(!CheckTrajectoryGoalReached())
            {
              // SetJointVelocities(InDeltaTime);
              UpdateDesiredJointAngle(InDeltaTime);
            }
        }
      CallculateJointVelocities(InDeltaTime);
      MoveJoints(InDeltaTime);
      break;

    case UJointControllerState::Normal:
      CallculateJointVelocities(InDeltaTime);
      MoveJoints(InDeltaTime);

      break;

    case UJointControllerState::Off:

      break;
    }
}

void URJointController::MoveJoints(float InDeltaTime)
{
  switch(Mode)
    {
    case UJointControllerMode::Kinematic:
      MoveJointsKinematic();
      break;

    case UJointControllerMode::Dynamic:
      MoveJointsDynamic(InDeltaTime);
      break;
    }
}

void URJointController::MoveJointsDynamic(float InDeltaTime)
{
  if(GetOwner()->Links.Contains(BaseLink))
    {
      GetOwner()->Links[BaseLink]->UpdateVelocity(InDeltaTime);
    }
}

void URJointController::MoveJointsKinematic()
{

  FHitResult * HitResult = nullptr;
  for(auto& Joint : GetOwner()->Joints)
    {
      if(DesiredJointState.Contains(Joint.Key))
        {
          Joint.Value->SetJointPosition(DesiredJointState[Joint.Key], HitResult);
        }
    }
}

void URJointController::Init()
{
  State = UJointControllerState::Normal;
  bPublishResult = false;
  if(!GetOwner())
    {
      UE_LOG(LogTemp, Error, TEXT("JointController not attached to ARModel"));
    }
  else
    {
      SwitchMode(Mode, true);
      for(auto & Link: GetOwner()->Links)
        {
          Link.Value->GetCollision()->SetEnableGravity(false);
          if(bDisableCollision)
          {
            Link.Value->DisableCollision();
          }
        }

      // for(auto & Joint: GetOwner()->Joints)
      //   {
      //     if(Joint.Value->Constraint->IsA(URContinuousConstraintComponent::StaticClass()) or
      //        Joint.Value->Constraint->IsA(URRevoluteConstraintComponent::StaticClass()))
      //       {
      //         Joint.Value->Constraint->JointAccuracy = RevolutAccuracy;
      //       }
      //     else if(Joint.Value->Constraint->IsA(URPrismaticConstraintComponent::StaticClass()))
      //       {
      //         Joint.Value->Constraint->JointAccuracy = PrismaticAccuracy;
      //       }
        // }
    }
}

void URJointController::SwitchMode(UJointControllerMode InMode, bool IsInit)
{
  if(Mode == InMode && !IsInit)
    {
      return;
    }

  Mode = InMode;
  if(!GetOwner())
    {
      return;
    }

  bool bEnablePhysics;
  switch(Mode)
    {
    case UJointControllerMode::Kinematic:
      bEnablePhysics = false;
      break;

    case UJointControllerMode::Dynamic:
      bEnablePhysics = true;
      break;

	default:
		bEnablePhysics = true;
    }

  for(auto& Joint : GetOwner()->Joints)
    {
      Joint.Value->Child->GetCollision()->SetSimulatePhysics(bEnablePhysics);
    }
}

void URJointController::FollowTrajectory()
{
  TrajectoryPointIndex = 0;
  OldTrajectoryPoints.Reset();
  URJoint* Joint = nullptr;
  for(auto& JointName : TrajectoryStatus.JointNames)
    {
      Joint = GetOwner()->Joints[JointName];
      if(Joint)
        {
          OldTrajectoryPoints.Points.Add(Joint->GetEncoderValue());
        }
    }
  State = UJointControllerState::FollowJointTrajectory;
}

UJointControllerState URJointController::GetState()
{
  return State;
}
