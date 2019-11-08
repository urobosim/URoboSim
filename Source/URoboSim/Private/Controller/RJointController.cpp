
#include "Controller/RJointController.h"

URJointController::URJointController()
{
  BaseLink = TEXT("base_footprint");
  IgnoreList.Add(TEXT("l_gripper_l_finger_tip_joint"));
  IgnoreList.Add(TEXT("l_gripper_motor_slider_joint"));
  IgnoreList.Add(TEXT("l_gripper_motor_screw_joint"));
  IgnoreList.Add(TEXT("l_gripper_r_finger_tip_joint"));
  IgnoreList.Add(TEXT("r_gripper_l_finger_tip_joint"));
  IgnoreList.Add(TEXT("r_gripper_motor_slider_joint"));
  IgnoreList.Add(TEXT("r_gripper_motor_screw_joint"));
  IgnoreList.Add(TEXT("r_gripper_r_finger_tip_joint"));
  IgnoreList.Add(TEXT("l_gripper_joint"));
  IgnoreList.Add(TEXT("r_gripper_joint"));
  IgnoreList.Add(TEXT("r_gripper_r_parallel_root_joint"));
  IgnoreList.Add(TEXT("r_gripper_l_parallel_root_joint"));
  IgnoreList.Add(TEXT("l_gripper_r_parallel_root_joint"));
  IgnoreList.Add(TEXT("l_gripper_l_parallel_root_joint"));
  IgnoreList.Add(TEXT("r_gripper_r_parallel_tip_joint"));
  IgnoreList.Add(TEXT("r_gripper_l_parallel_tip_joint"));
  IgnoreList.Add(TEXT("l_gripper_r_parallel_tip_joint"));
  IgnoreList.Add(TEXT("l_gripper_l_parallel_tip_joint"));
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
  for(int i = 0; i < TrajectoryStatus.JointNames.Num(); i++)
    {
      URJoint* Joint = Model->Joints[TrajectoryStatus.JointNames[i]];
      if(Joint)
        {
          Joint->DesiredJointPose = Trajectory[TrajectoryPointIndex].Points[i];
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("%s of Trajectory not contained in RobotModel"), *TrajectoryStatus.JointNames[i]);
        }
    }
}

bool URJointController::CheckTrajectoryStatus()
{
  bool bFinalTrajectoryPointReached = false;

  // TrajectoryPointIndex++;

  if(TrajectoryPointIndex == Trajectory.Num()-1)
    {
      State = UJointControllerState::Normal;
      bPublishResult = true;

      GoalStatusList.Last().Status = 3;

      // FString FinalPos;
      // for(auto& pos : TrajectoryStatus.Position)
      //   {
      //     FinalPos.Append(FString::SanitizeFloat(pos));
      //     FinalPos.Append(" ");
      //   }
      // UE_LOG(LogTemp, Error, TEXT("Final pos: %s"), *FinalPos);

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
              // float CurrentJointPos = Joint->GetJointPosition();
              float CurrentJointPos = Joint->GetEncoderValue();
              float DesiredPos = Trajectory[TrajectoryPointIndex].Points[i];
              // DesiredPos = Joint->Constraint->CheckPositionRange(DesiredPos);
              float Diff = DesiredPos - CurrentJointPos;
              // Diff = Joint->Constraint->CheckPositionRange(Diff);

              if(FMath::Abs(Diff) > Joint->Constraint->JointAccuracy)
                {
                  bAllPointsReady = false;
                  UE_LOG(LogTemp, Error, TEXT("JointName %s Diff %f"), *TrajectoryStatus.JointNames[i], Diff);
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
      if(!IgnoreList.Contains(Joint.Key))
        {
          // float CurrentJointPos = Joint.Value->GetJointPosition();
          float CurrentJointPos = Joint.Value->GetEncoderValue();
          float DesiredPos = Joint.Value->DesiredJointPose;
          // DesiredPos = Joint.Value->Constraint->CheckPositionRange(DesiredPos);
          float Diff = DesiredPos - CurrentJointPos;

          // Diff = Joint.Value->Constraint->CheckPositionRange(Diff);

          float Vel = Diff / InDeltaTime;
          if(Joint.Value->MaxJointVel > 0)
            {
              if(FMath::Abs(Vel) > Joint.Value->MaxJointVel)
                {
                  Vel = Vel / FMath::Abs(Vel) * Joint.Value->MaxJointVel;
                }
            }
      // Velocity.Append(FString::SanitizeFloat(Vel));
      // Velocity.Append(" ");

          Joint.Value->SetJointVelocity(Vel);
        }
    }
  // UE_LOG(LogTemp, Error, TEXT("Vel: %s"), *Velocity);
}


void URJointController::Tick(float InDeltaTime)
{
  if(!Model)
    {
      UE_LOG(LogTemp, Error, TEXT("Model not initialized"));
      return;
    }

  if(Model->Links.Contains(BaseLink))
    {
      Model->Links[BaseLink]->UpdateEncoder();
    }

  // if(State == UJointControllerState::FollowJointTrajectory)
  //   {
  //     if(!CheckTrajectoryStatus())
  //       {
  //         UpdateDesiredJointAngle(InDeltaTime);
  //       }
  //   }

  // for(auto& Joint : Model->Joints)
  //   {
  //     Joint.Value->Constraint->SetTargetPosition(Joint.Value->DesiredJointPose);
  //   }

  switch(State)
    {
    case UJointControllerState::FollowJointTrajectory:
      if(!CheckTrajectoryStatus())
        {
          UpdateDesiredJointAngle(InDeltaTime);
          CallculateJointVelocities(InDeltaTime);
          MoveJoints();
        }
      break;

    case UJointControllerState::Normal:
      for(auto& Joint : Model->Joints)
        {
          // Joint.Value->Constraint->SetTargetPosition(Joint.Value->DesiredJointPose);
          MoveJoints();
        }

      break;

    case UJointControllerState::Off:

      break;
    }
}

void URJointController::MoveJoints()
{
  switch(Mode)
    {
    case UJointControllerMode::Kinematic:
      MoveJointsKinematic();
      break;

    case UJointControllerMode::Dynamic:
      MoveJointsDynamic();
      break;
    }
}

void URJointController::MoveJointsDynamic()
{
  if(Model->Links.Contains(BaseLink))
    {
      Model->Links[BaseLink]->UpdateVelocity();
    }
}

void URJointController::MoveJointsKinematic()
{
  for(auto& Joint : Model->Joints)
    {
      if(DesiredJointState.Contains(Joint.Key))
      {
        Joint.Value->SetJointPosition(DesiredJointState[Joint.Key]);
      }
    }
}

void URJointController::Init(ARModel* InModel)
{
  State = UJointControllerState::Normal;
  bPublishResult = false;
  if(!InModel)
    {
      UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
      // Status = 4;
    }
  else
    {
      Model = InModel;
      SwitchMode(Mode, true);
      for(auto & Link: Model->Links)
        {
          Link.Value->GetCollision()->SetEnableGravity(false);
        }
      Model->Links[BaseLink]->GetCollision()->SetEnableGravity(false);
      // for(auto & IgnoredJoint: IgnoreList)
      //   {
      //     if(Model->Joints.Contains(IgnoredJoint))
      //       {
      //         Model->Joints[IgnoredJoint]->bActuate = false;
      //       }

      //   }


      // float Stiffness = 1000000000;
      // float Dampening = 1000000000;
      // float MaxForce = 1000000;
      // for(auto & Joint: Model->Joints)
      //   {
      //     if(!IgnoreList.Contains(Joint.Key))
      //       {
      //         Joint.Value->Constraint->SetLinearDriveParams(Stiffness, Dampening, MaxForce);
      //         Joint.Value->Constraint->SetAngularDriveParams(Stiffness, Dampening, MaxForce);
      //         Joint.Value->EnableMotor(true);
      //         // Joint.Value->bActuate = true;
      //       }
      //     else
      //       {
      //         UE_LOG(LogTemp, Error, TEXT("Joint %s ignored"), *Joint.Key);
      //       }
      //   }
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
