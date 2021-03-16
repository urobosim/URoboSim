#include "Controller/ControllerType/RJointController.h"

URJointController::URJointController()
{
  State = UJointControllerState::Normal;
  Mode = UJointControllerMode::Dynamic;
}

void URJointController::BeginPlay()
{
  State = UJointControllerState::Normal;
  bPublishResult = false;
  if (!GetOwner())
  {
    UE_LOG(LogTemp, Error, TEXT("JointController is not attached to ARModel"));
  }
  else
  {
    SetMode();
    for (URJoint *&Joint : GetOwner()->GetJoints())
    {
      DesiredJointStates.Add(Joint->GetName(), FJointState());
    }
  }
}

void URJointController::SetMode()
{
  if (!GetOwner())
  {
    return;
  }
  else
  {
    switch (Mode)
    {
    case UJointControllerMode::Kinematic:
      GetOwner()->bSimulatePhysics = false;
      for (URLink *&Link : GetOwner()->GetLinks())
      {
        Link->DisableCollision();
      }
      break;

    case UJointControllerMode::Dynamic:
      GetOwner()->bSimulatePhysics = true;
      GetOwner()->EnableGravity.bLinks = false;
      for (URJoint *&Joint : GetOwner()->GetJoints())
      {
        Joint->SetDrive(EnableDrive);
      }
      if (bDisableCollision)
      {
        for (URLink *&Link : GetOwner()->GetLinks())
        {
          Link->DisableCollision();
        }
      }
      break;
    }
  }
}

void URJointController::Tick(float DeltaTime)
{
  if (!GetOwner())
  {
    return;
  }
  else
  {
    switch (State)
    {
    case UJointControllerState::FollowJointTrajectory:
      // ActionDuration += InDeltaTime * SpeedFactorHack;
      // if (!CheckTrajectoryPoint())
      // {
      //   UpdateDesiredJointAngle(InDeltaTime);
      //   // SetJointVelocities(InDeltaTime);
      // }
      // else
      // {
      //   if (!CheckTrajectoryGoalReached())
      //   {
      //     // SetJointVelocities(InDeltaTime);
      //     UpdateDesiredJointAngle(InDeltaTime);
      //   }
      // }
      // CallculateJointVelocities(InDeltaTime);
      // MoveJoints(InDeltaTime);
      break;

    case UJointControllerState::Normal:
      SetJointState(DeltaTime);
      break;

    case UJointControllerState::Off:
      break;
    }
  }
}

void URJointController::SetJointState(float DeltaTime)
{
  for (URJoint *&Joint : GetOwner()->GetJoints())
  {
    if (DesiredJointStates.Contains(Joint->GetName()))
    {
      Joint->SetTargetPosition(DesiredJointStates[Joint->GetName()].JointPosition);
      Joint->SetTargetVelocity(DesiredJointStates[Joint->GetName()].JointVelocity);
    }
  }
}

// void URJointController::SetJointNames(TArray<FString> InNames)
// {
//   TrajectoryStatus.JointNames = InNames;
//   int JointNum = InNames.Num();
//   TrajectoryStatus.Error.Empty();
//   TrajectoryStatus.Error.AddDefaulted(JointNum);
//   TrajectoryStatus.Desired.Empty();
//   TrajectoryStatus.Desired.AddDefaulted(JointNum);
//   TrajectoryStatus.Position.Empty();
//   TrajectoryStatus.Position.AddDefaulted(JointNum);
//   bTrajectoryPointsReached.Empty();
//   bTrajectoryPointsReached.AddDefaulted(JointNum);
//   ActionDuration = 0.0;
//   TrajectoryPointIndex = 0;
//   Trajectory.Empty();
// }

// void URJointController::SetJointVelocities(float InDeltaTime)
// {
//   if (State == UJointControllerState::FollowJointTrajectory)
//   {
//     for (int i = 0; i < TrajectoryStatus.JointNames.Num(); i++)
//     {
//       URJoint *Joint = GetOwner()->Joints[TrajectoryStatus.JointNames[i]];
//       if (Joint)
//       {
//         Joint->SetJointVelocity(Trajectory[TrajectoryPointIndex].Velocities[i]);
//       }
//     }
//   }
// }

// void URJointController::UpdateDesiredJointAngle(float InDeltaTime)
// {
//   if (State == UJointControllerState::FollowJointTrajectory)
//   {

//     float NextTimeStep = Trajectory[TrajectoryPointIndex].GetTimeAsDouble();
//     float OldTimeStep = OldTrajectoryPoints.GetTimeAsDouble();
//     float CurrentTimeStep = ActionDuration;
//     float DiffTrajectoryTimeStep = NextTimeStep - OldTimeStep;

//     //if execution is slower than the trajectory demands, use the NextTimeStep for further calculations
//     //if execution is faster than the trajectory demands, use the OldTimeStep for further calculations
//     // in order to cap the linear interpolation between the points
//     if (ActionDuration < OldTimeStep)
//     {
//       CurrentTimeStep = OldTimeStep;
//     }
//     else if (ActionDuration > NextTimeStep)
//     {
//       CurrentTimeStep = NextTimeStep;
//     }

//     for (int i = 0; i < TrajectoryStatus.JointNames.Num(); i++)
//     {
//       FString JointName = TrajectoryStatus.JointNames[i];
//       float &JointState = DesiredJointState.FindOrAdd(JointName);
//       float DiffJointStep;
//       DiffJointStep = Trajectory[TrajectoryPointIndex].Points[i] - OldTrajectoryPoints.Points[i];
//       JointState = DiffJointStep / DiffTrajectoryTimeStep * (CurrentTimeStep - OldTimeStep) + OldTrajectoryPoints.Points[i];
//     }
//   }
// }

// bool URJointController::CheckTrajectoryPoint()
// {
//   bool bAllPointsReady = true;
//   float NextTimeStep = Trajectory[TrajectoryPointIndex].GetTimeAsDouble();
//   if (NextTimeStep == 0)
//   {
//     TrajectoryPointIndex++;
//   }

//   for (int i = 0; i < TrajectoryStatus.JointNames.Num(); i++)
//   {
//     URJoint *Joint = GetOwner()->Joints[TrajectoryStatus.JointNames[i]];
//     if (Joint)
//     {
//       float CurrentJointPos = Joint->GetEncoderValue();
//       float DesiredPos = Trajectory[TrajectoryPointIndex].Points[i];
//       float Diff = DesiredPos - CurrentJointPos;

//       TrajectoryStatus.Position[i] = CurrentJointPos;
//       TrajectoryStatus.Desired[i] = DesiredPos;
//       TrajectoryStatus.Error[i] = Diff;
//     }
//     else
//     {
//       UE_LOG(LogTemp, Error, TEXT("%s of Trajectory not contained in RobotModel"), *TrajectoryStatus.JointNames[i]);
//     }
//   }

//   GoalStatusList.Last().Status = 1;

//   if (ActionDuration > NextTimeStep)
//   {
//     float CurrentTimeStep = ActionDuration;
//     OldTrajectoryPoints = Trajectory[TrajectoryPointIndex];
//     TrajectoryPointIndex++;
//     return true;
//   }

//   return false;
// }

// bool URJointController::CheckTrajectoryGoalReached()
// {
//   if (TrajectoryPointIndex == Trajectory.Num())
//   {
//     State = UJointControllerState::Normal;
//     bPublishResult = true;

//     GoalStatusList.Last().Status = 3;

//     Trajectory.Empty();
//     TrajectoryPointIndex = 0;
//     ActionDuration = 0.0;
//     return true;
//   }

//   return false;
// }

// void URJointController::SetDesiredJointState(FString JointName, float InJointState)
// {
//   URJoint *Joint = GetOwner()->Joints[JointName];
//   if (Joint)
//   {
//     float &JointValue = DesiredJointState.FindOrAdd(JointName);
//     // UE_LOG(LogTemp, Warning, TEXT("JointName %s Upper %f Lower %f"), *JointName, Joint->Constraint->Upper, Joint->Constraint->Lower);
//     //TODO: Remove hardcode

//     JointValue = Joint->Constraint->ClampJointStateToConstraintLimit(InJointState);
//   }
//   else
//   {
//     UE_LOG(LogTemp, Error, TEXT("Setting DesiredJointState failed. Joint %s not contained in Model"), *JointName);
//   }
// }

// void URJointController::MoveJoints(float InDeltaTime)
// {
//   switch (Mode)
//   {
//   case UJointControllerMode::Kinematic:
//     MoveJointsKinematic();
//     break;

//   case UJointControllerMode::Dynamic:
//     MoveJointsDynamic(InDeltaTime);
//     break;
//   }
// }

// void URJointController::MoveJointsDynamic(float InDeltaTime)
// {
//   if (GetOwner()->Links.Contains(BaseLink))
//   {
//     GetOwner()->Links[BaseLink]->UpdateVelocity(InDeltaTime);
//   }
// }

// void URJointController::MoveJointsKinematic()
// {

//   FHitResult *HitResult = nullptr;
//   for (auto &Joint : GetOwner()->Joints)
//   {
//     if (DesiredJointState.Contains(Joint.Key))
//     {
//       Joint.Value->SetJointPosition(DesiredJointState[Joint.Key], HitResult);
//     }
//   }
// }

// void URJointController::FollowTrajectory()
// {
//   TrajectoryPointIndex = 0;
//   OldTrajectoryPoints.Reset();
//   URJoint *Joint = nullptr;
//   for (auto &JointName : TrajectoryStatus.JointNames)
//   {
//     Joint = GetOwner()->Joints[JointName];
//     if (Joint)
//     {
//       OldTrajectoryPoints.Points.Add(Joint->GetEncoderValue());
//     }
//   }
//   State = UJointControllerState::FollowJointTrajectory;
// }

// UJointControllerState URJointController::GetState()
// {
//   return State;
// }