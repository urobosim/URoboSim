
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
}

void URJointController::UpdateDesiredJointAngle(float InDeltaTime)
{
  if(State == UJointControllerState::FollowJointTrajectory)
    {
      for(int i = 0; i < TrajectoryStatus.JointNames.Num(); i++)
        {
          FString JointName = TrajectoryStatus.JointNames[i];
          float& JointState = DesiredJointState.FindOrAdd(JointName);

          JointState = Trajectory[TrajectoryPointIndex].Points[i];
        }
    }
}

bool URJointController::CheckTrajectoryStatus()
{
  bool bFinalTrajectoryPointReached = false;

  if(TrajectoryPointIndex == Trajectory.Num()-1)
    {
      State = UJointControllerState::Normal;
      bPublishResult = true;

      GoalStatusList.Last().Status = 3;

      Trajectory.Empty();
      TrajectoryPointIndex = 0;
      ActionDuration = 0.0;
      bFinalTrajectoryPointReached = true;
    }
  else
    {
      bool bAllPointsReady = true;
      for(int i = 0; i < TrajectoryStatus.JointNames.Num(); i++)
        {
          URJoint* Joint = Model->Joints[TrajectoryStatus.JointNames[i]];
          if(Joint)
            {
              float CurrentJointPos = Joint->GetEncoderValue();
              float DesiredPos = Trajectory[TrajectoryPointIndex].Points[i];
              float Diff = DesiredPos - CurrentJointPos;

              if(FMath::Abs(Diff) > Joint->Constraint->JointAccuracy)
                {
                  bAllPointsReady = false;
                }
              TrajectoryStatus.Position[i] = CurrentJointPos;
              TrajectoryStatus.Desired[i] = Trajectory[TrajectoryPointIndex].Points[i];
              TrajectoryStatus.Error[i] = Diff;
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("%s of Trajectory not contained in RobotModel"), *TrajectoryStatus.JointNames[i]);
            }
        }
      FString Error = "";
      for(auto& error : TrajectoryStatus.Error)
        {
          Error.Append(FString::SanitizeFloat(error));
          Error.Append(" ");
        }

      if(bAllPointsReady)
        {
          TrajectoryPointIndex++;
        }

      GoalStatusList.Last().Status = 1;
    }

  return bFinalTrajectoryPointReached;
}
void URJointController::CallculateJointVelocities(float InDeltaTime)
{
  FString Velocity = "";
  for(auto & Joint: Model->Joints)
    {
      if(DesiredJointState.Contains(Joint.Key))
        {
          Joint.Value->bActuate = true;
          float DesiredPos = 0.0f;
          DesiredPos = DesiredJointState[Joint.Key];

          float CurrentJointPos = Joint.Value->GetEncoderValue();
          float Diff = DesiredPos - CurrentJointPos;
          Diff = Joint.Value->Constraint->CheckPositionRange(Diff);

          float Vel = Diff / InDeltaTime;
          if(!Joint.Key.Equals("torso_lift_joint"))
            {
              Joint.Value->MaxJointVel = MaxJointAngularVel;
            }
          if(Joint.Value->MaxJointVel > 0)
            {
              if(FMath::Abs(Vel) > Joint.Value->MaxJointVel)
                {
                  Vel = Vel / FMath::Abs(Vel) * Joint.Value->MaxJointVel;
                }
            }
          Joint.Value->SetJointVelocity(Vel);
        }
    }
}


void URJointController::Tick(float InDeltaTime)
{
  if(!Model)
    {
      UE_LOG(LogTemp, Error, TEXT("Model not initialized"));
      return;
    }

  for(auto& Joint : Model->Joints)
    {
      Joint.Value->UpdateEncoder();
    }

  switch(State)
    {
    case UJointControllerState::FollowJointTrajectory:
      if(!CheckTrajectoryStatus())
        {
          UpdateDesiredJointAngle(InDeltaTime);
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
  if(Model->Links.Contains(BaseLink))
    {
      Model->Links[BaseLink]->UpdateVelocity(InDeltaTime);
    }
}

void URJointController::MoveJointsKinematic()
{

  FHitResult * HitResult = nullptr;
  for(auto& Joint : Model->Joints)
    {
      if(DesiredJointState.Contains(Joint.Key))
        {
          Joint.Value->SetJointPosition(DesiredJointState[Joint.Key], HitResult);
        }
    }
}

void URJointController::Init(ARModel* InModel)
{
  State = UJointControllerState::Normal;
  bPublishResult = false;
  if(!InModel)
    {
      UE_LOG(LogTemp, Error, TEXT("JointController not attached to ARModel"));
    }
  else
    {
      Model = InModel;
      SwitchMode(Mode, true);
      for(auto & Link: Model->Links)
        {
          Link.Value->GetCollision()->SetEnableGravity(false);
        }
    }
}

void URJointController::SwitchMode(UJointControllerMode InMode, bool IsInit)
{
  if(Mode == InMode && !IsInit)
    {
      return;
    }

  Mode = InMode;
  if(!Model)
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
    }

  for(auto& Joint : Model->Joints)
    {
      Joint.Value->Child->GetCollision()->SetSimulatePhysics(bEnablePhysics);
    }
}

void URJointController::FollowTrajectory()
{
  TrajectoryPointIndex = 0;
  State = UJointControllerState::FollowJointTrajectory;
}

UJointControllerState URJointController::GetState()
{
  return State;
}
