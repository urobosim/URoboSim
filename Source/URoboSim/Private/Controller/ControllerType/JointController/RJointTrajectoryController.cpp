#include "Controller/ControllerType/JointController/RJointTrajectoryController.h"
#include "ROSTime.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJointTrajectoryController, Log, All);

URJointTrajectoryController::URJointTrajectoryController()
{
  Mode = UJointControllerMode::Dynamic;
}

void URJointTrajectoryController::Reset()
{
  ActionDuration = 0.0;
  TrajectoryPointIndex = 0;
  Trajectory.Empty();
  TrajectoryPointIndex = 0;
  OldTrajectoryPoints.Reset();
}

void URJointTrajectoryController::SetJointVelocities(float InDeltaTime)
{
  if (State == UJointControllerState::FollowJointTrajectory)
  {
    for (const TPair<FString, FJointState> &DesiredJointState : DesiredJointStates)
    {
      FString JointName = DesiredJointState.Key;
      if (URJoint *Joint = GetOwner()->GetJoint(JointName))
      {
        Joint->SetJointVelocity(Trajectory[TrajectoryPointIndex].JointStates[JointName].JointVelocity);
      }
    }
  }
}

void URJointTrajectoryController::UpdateDesiredJointAngle(float InDeltaTime)
{
  if (State == UJointControllerState::FollowJointTrajectory)
  {
    float NextTimeStep = Trajectory[TrajectoryPointIndex].GetTimeAsDouble();
    float OldTimeStep = OldTrajectoryPoints.GetTimeAsDouble();
    float CurrentTimeStep = ActionDuration;
    float DiffTrajectoryTimeStep = NextTimeStep - OldTimeStep;

    //if execution is slower than the trajectory demands, use the NextTimeStep for further calculations
    //if execution is faster than the trajectory demands, use the OldTimeStep for further calculations
    // in order to cap the linear interpolation between the points
    if (ActionDuration < OldTimeStep)
    {
      CurrentTimeStep = OldTimeStep;
    }
    else if (ActionDuration > NextTimeStep)
    {
      CurrentTimeStep = NextTimeStep;
    }

    for (TPair<FString, FJointState> &DesiredJointState : DesiredJointStates)
    {
      FString JointName = DesiredJointState.Key;
      FJointState &JointState = DesiredJointState.Value;
      if (Trajectory[TrajectoryPointIndex].JointStates.Contains(JointName))
      {
        float DiffJointPosition = Trajectory[TrajectoryPointIndex].JointStates[JointName].JointPosition - OldTrajectoryPoints.JointStates[JointName].JointPosition;
        float DiffJointVelocity = CalculateJointVelocity(InDeltaTime, JointName);
        JointState.JointPosition = DiffJointPosition / DiffTrajectoryTimeStep * (CurrentTimeStep - OldTimeStep) + OldTrajectoryPoints.JointStates[JointName].JointPosition;
        JointState.JointVelocity = DiffJointVelocity;
      }
    }
  }
}

bool URJointTrajectoryController::CheckTrajectoryPoint()
{
  bool bAllPointsReady = true;
  float NextTimeStep = Trajectory[TrajectoryPointIndex].GetTimeAsDouble();
  if (NextTimeStep == 0)
  {
    TrajectoryPointIndex++;
  }

  GoalStatusList.Last().Status = 1;

  if (ActionDuration >= NextTimeStep)
  {
    // UE_LOG(LogRJointTrajectoryController, Error, TEXT("Actionduration %f NextTimeStep %f"), ActionDuration, NextTimeStep);
    float CurrentTimeStep = ActionDuration;
    OldTrajectoryPoints = Trajectory[TrajectoryPointIndex];
    TrajectoryPointIndex++;
    return true;
  }
  else
  {
    return false;
  }
}

bool URJointTrajectoryController::CheckTrajectoryGoalReached()
{
  if (TrajectoryPointIndex >= Trajectory.Num())
  {
    // State = UJointControllerState::Normal;
    SwitchToNormal();

    GoalStatusList.Last().Status = 3;
    if( ActionFinished.IsBound() )
      {
        UE_LOG(LogTemp, Error, TEXT("JointTrjac Result is bound"));
      }
    else
      {
        UE_LOG(LogTemp, Error, TEXT("JointTrjac Result not bound"));
      }

    ActionFinished.Broadcast(GoalStatusList.Last());

    Trajectory.Empty();
    TrajectoryPointIndex = 0;
    ActionDuration = 0.0;
    return true;
  }
  else
  {
    return false;
  }
}

float URJointTrajectoryController::CalculateJointVelocity(float InDeltaTime, FString InJointName)
{
  FString Velocity = "";

  float DesiredPos = DesiredJointStates[InJointName].JointPosition;
  URJoint *Joint = GetOwner()->Joints[InJointName];
  float CurrentJointPos = Joint->GetEncoderValue();
  float Diff = DesiredPos - CurrentJointPos;
  // Diff = Joint->Constraint->CheckPositionRange(Diff);

  // float Vel = Diff / InDeltaTime;
  float Vel = Diff;
  float VelSave = Vel;
  return Vel;

  // for(auto & Joint: GetOwner()->Joints)
  //   {
  //     if(DesiredJointState.Contains(Joint.Key))
  //       {
  //         Joint.Value->bActuate = true;
  //         float DesiredPos = 0.0f;
  //         DesiredPos = DesiredJointState[Joint.Key];

  //         float CurrentJointPos = Joint.Value->GetEncoderValue();
  //         float Diff = DesiredPos - CurrentJointPos;

  //         Diff = Joint.Value->Constraint->CheckPositionRange(Diff);

  //         float Vel = Diff / InDeltaTime;
  //         float VelSave = Vel;
  //         Joint.Value->SetJointVelocity(Vel);
  //       }
  //   }
}

void URJointTrajectoryController::SetDesiredJointState(FString JointName, float InJointState)
{
  if (URJoint *Joint = GetOwner()->GetJoint(JointName))
  {
    float &JointValue = DesiredJointStates.FindOrAdd(JointName).JointPosition;
    // UE_LOG(LogTemp, Warning, TEXT("JointName %s Upper %f Lower %f"), *JointName, Joint->Constraint->Upper, Joint->Constraint->Lower);
    //TODO: Remove hardcode

    JointValue = Joint->Constraint->ClampJointStateToConstraintLimit(InJointState);
  }
  else
  {
    UE_LOG(LogRJointTrajectoryController, Error, TEXT("Setting DesiredJointState failed. Joint %s not contained in Model"), *JointName);
  }
}

void URJointTrajectoryController::Tick(const float &InDeltaTime)
{
  if (GetOwner())
  {
    // for (URJoint *&Joint : GetOwner()->GetJoints())
    // {
    //   Joint->UpdateEncoder();
    // }

    if (State == UJointControllerState::FollowJointTrajectory)
    {
      ActionDuration += SpeedFactor * InDeltaTime;
      if (!CheckTrajectoryPoint() || !CheckTrajectoryGoalReached())
      {
        UpdateDesiredJointAngle(InDeltaTime);
      }
    }
    MoveJoints(InDeltaTime);
  }
}

void URJointTrajectoryController::FollowJointTrajectory(double InActionStartTime, FGoalStatusInfo InGoalInfo, TArray<FTrajectoryPoints> InTrajectory)
{
  UE_LOG(LogRJointTrajectoryController, Error, TEXT("FollowTrajectory"));
  Reset();
  SwitchToFollowJointTrajectory();
  for (const TPair<FString, FJointState> &DesiredJointState : DesiredJointStates)
  {
    if (URJoint *Joint = GetOwner()->GetJoint(DesiredJointState.Key))
    {
      OldTrajectoryPoints.JointStates.Add(DesiredJointState.Key, FJointState(Joint->GetEncoderValue(), Joint->GetJointVelocity()));
    }
  }
  Trajectory = InTrajectory;
  GoalStatusList.Add(InGoalInfo);
  // State = UJointControllerState::FollowJointTrajectory;
}

bool URJointTrajectoryController::SwitchToNormal()
{
  switch (State)
  {
  case UJointControllerState::FollowJointTrajectory:
    State = UJointControllerState::Normal;
    break;

  case UJointControllerState::Normal:
    UE_LOG(LogRJointTrajectoryController, Warning, TEXT("Trajectory already in NormalMode."));
    break;
  }

  return true;
}

bool URJointTrajectoryController::SwitchToFollowJointTrajectory()
{
  switch (State)
  {
  case UJointControllerState::FollowJointTrajectory:
    UE_LOG(LogRJointTrajectoryController, Warning, TEXT("Trajectory already in Progress."));
    GoalStatusList.Last().Status = 4;
    ActionFinished.Broadcast(GoalStatusList.Last());
    Trajectory.Empty();
    TrajectoryPointIndex = 0;
    ActionDuration = 0.0;
    break;

  case UJointControllerState::Normal:
    State = UJointControllerState::FollowJointTrajectory;
    break;
  }

  return true;
}
