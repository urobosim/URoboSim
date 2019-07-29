
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
      bFollowTrajectory = false;
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
      // FString Name = "";
      // for(auto& pos : TrajectoryStatus.JointNames)
      //   {
      //     Name.Append(pos);
      //     Name.Append(" ");
      //   }

      // FString Position = "";
      // for(auto& pos : TrajectoryStatus.Position)
      //   {
      //     Position.Append(FString::SanitizeFloat(pos));
      //     Position.Append(" ");
      //   }
      // UE_LOG(LogTemp, Position, TEXT("Position: %s"), *Position);

      // FString Desired = "";
      // for(auto& desired : TrajectoryStatus.Desired)
      //   {
      //     Desired.Append(FString::SanitizeFloat(desired));
      //     Desired.Append(" ");
      //   }
      // UE_LOG(LogTemp, Error, TEXT("Desired: %s"), *Desired);
      FString Error = "";
      for(auto& error : TrajectoryStatus.Error)
        {
          Error.Append(FString::SanitizeFloat(error));
          Error.Append(" ");
        }
      // UE_LOG(LogTemp, Error, TEXT("Error: %s"), *Error);

      if(bAllPointsReady)
        {
          TrajectoryPointIndex++;
          // UE_LOG(LogTemp, Error, TEXT("TrajIndex: %d/ %d"), TrajectoryPointIndex, Trajectory.Num());
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

  if(bFollowTrajectory)
    {
      if(!CheckTrajectoryStatus())
        {
          UpdateDesiredJointAngle(InDeltaTime);
        }
    }

  CallculateJointVelocities(InDeltaTime);

  if(Model->Links.Contains(BaseLink))
    {
      Model->Links[BaseLink]->UpdateVelocity();
      // Model->Links[BaseLink]->UpdateJointStates();
    }
}

void URJointController::Init(ARModel* InModel)
{
  bFollowTrajectory = false;
  bPublishResult = false;
  if(!InModel)
    {
      UE_LOG(LogTemp, Error, TEXT("RobotComandsComponent not attached to ARModel"));
      // Status = 4;
    }
  else
    {
      Model = InModel;
      for(auto & Link: Model->Links)
        {
          Link.Value->GetCollision()->SetEnableGravity(false);
        }
      Model->Links[BaseLink]->GetCollision()->SetEnableGravity(false);
      for(auto & IgnoredJoint: IgnoreList)
        {
          if(Model->Joints.Contains(IgnoredJoint))
            {
              Model->Joints[IgnoredJoint]->bActuate = false;
            }

        }
      // Status = 1;
    }
}

void URJointController::FollowTrajectory()
{
	TrajectoryPointIndex = 0;
	bFollowTrajectory = true;
}
