#include "Controller/ControllerType/RJointController.h"
#include "Conversions.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJointController, Log, All);

URJointController::URJointController()
{
  State = UJointControllerState::Normal;
  Mode = UJointControllerMode::Dynamic;
  bDisableCollision = true;
  EnableDrive.PositionStrength = 1E5;
  EnableDrive.VelocityStrength = 1E5;
  EnableDrive.MaxForce = 1E10;
}

void URJointController::SetJointNames(const TArray<FString> &InNames)
{
  TrajectoryStatusArray.Empty();
  for (const FString &InName : InNames)
  {
    TrajectoryStatusArray.Add(FTrajectoryStatus(InName));
  }
  TrajectoryPointIndex = 0;
  DesiredTrajectory.Empty();
}

void URJointController::Init()
{
  Super::Init();

  State = UJointControllerState::Normal;
  bPublishResult = false;
  if (GetOwner())
  {
    SetMode();
    for (URJoint *&Joint : GetOwner()->GetJoints())
    {
      DesiredJointStates.Add(Joint->GetName(), FJointState());
    }
  }
  else
  {
    UE_LOG(LogRJointController, Error, TEXT("%s is not attached to ARModel"), *GetName())
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
      EnableDrive.bPositionDrive = false;
      EnableDrive.bVelocityDrive = false;
      for (URLink *&Link : GetOwner()->GetLinks())
      {
        Link->DisableCollision();
      }
      break;

    case UJointControllerMode::Dynamic:
      GetOwner()->bSimulatePhysics = true;
      GetOwner()->EnableGravity.bLinks = false;
      EnableDrive.bPositionDrive = true;
      EnableDrive.bVelocityDrive = true;
      if (bDisableCollision)
      {
        for (URLink *&Link : GetOwner()->GetLinks())
        {
          Link->DisableCollision();
        }
      }
      else
      {
        for (URLink *&Link : GetOwner()->GetLinks())
        {
          Link->EnableCollision();
        }
      }
      break;
    }
    for (URJoint *&Joint : GetOwner()->GetJoints())
    {
      Joint->SetDrive(EnableDrive);
    }
  }
}

void URJointController::Tick(const float &InDeltaTime)
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
      ActionDuration = GetOwner()->GetGameTimeSinceCreation() - ActionStartTime;
      if (!CheckTrajectoryPoint())
      {
        SetDesiredJointState();
      }
      else
      {
        if (!CheckTrajectoryGoalReached())
        {
          SetDesiredJointState();
        }
      }
      SetJointState();
      break;

    case UJointControllerState::Normal:
      SetJointState();
      break;

    case UJointControllerState::Off:
      break;
    }
  }
}

void URJointController::SetJointState()
{
  switch (Mode)
  {
    case UJointControllerMode::Dynamic:
      for (URJoint *&Joint : GetOwner()->GetJoints())
      {
        if (DesiredJointStates.Contains(Joint->GetName()))
        {
          Joint->SetTargetPosition(DesiredJointStates[Joint->GetName()].JointPosition);
          Joint->SetTargetVelocity(DesiredJointStates[Joint->GetName()].JointVelocity);
        }
      }
      break;

    case UJointControllerMode::Kinematic:
      for (URJoint *&Joint : GetOwner()->GetJoints())
      {
        if (DesiredJointStates.Contains(Joint->GetName()))
        {
          Joint->SetPosition(DesiredJointStates[Joint->GetName()].JointPosition);
        }
      }
      break;
  }
}

bool URJointController::CheckTrajectoryPoint()
{
  float NextTimeStep = DesiredTrajectory[TrajectoryPointIndex].GetTimeAsFloat();
  if (NextTimeStep == 0)
  {
    TrajectoryPointIndex++;
  }

  for (FTrajectoryStatus &TrajectoryStatus : TrajectoryStatusArray)
  {
    URJoint *Joint = GetOwner()->GetJoint(TrajectoryStatus.JointName);
    if (Joint)
    {
      TrajectoryStatus.CurrentPosition = Joint->GetJointStateInROSUnit().JointPosition;
      TrajectoryStatus.DesiredPosition = DesiredTrajectory[TrajectoryPointIndex].JointStates[TrajectoryStatus.JointName].JointPosition;
      TrajectoryStatus.ErrorPosition = TrajectoryStatus.DesiredPosition - TrajectoryStatus.CurrentPosition;
    }
    else
    {
      UE_LOG(LogRJointController, Error, TEXT("%s of DesiredTrajectory is not contained in %s"), *TrajectoryStatus.JointName, *GetOwner()->GetName());
    }
  }

  GoalStatusList.Last().Status = 1;

  if (ActionDuration > NextTimeStep)
  {
    LastTrajectoryPoints = DesiredTrajectory[TrajectoryPointIndex];
    TrajectoryPointIndex++;
    return true;
  }
  else
  {
    return false;
  }
}

void URJointController::SetDesiredJointState()
{
  if (State == UJointControllerState::FollowJointTrajectory)
  {
    float NextTimeStep = DesiredTrajectory[TrajectoryPointIndex].GetTimeAsFloat();
    float LastTimeStep = LastTrajectoryPoints.GetTimeAsFloat();
    float CurrentTimeStep = ActionDuration;
    float DiffTrajectoryTimeStep = NextTimeStep - LastTimeStep;

    // if execution is slower than the trajectory demands, use the NextTimeStep for further calculations
    // if execution is faster than the trajectory demands, use the LastTimeStep for further calculations
    // in order to cap the linear interpolation between the points
    if (ActionDuration < LastTimeStep)
    {
      CurrentTimeStep = LastTimeStep;
    }
    else if (ActionDuration > NextTimeStep)
    {
      CurrentTimeStep = NextTimeStep;
    }
    
    for (FTrajectoryStatus &TrajectoryStatus : TrajectoryStatusArray)
    {
      FString JointName = TrajectoryStatus.JointName;
      float DiffJointPosition = DesiredTrajectory[TrajectoryPointIndex].JointStates[JointName].JointPosition - LastTrajectoryPoints.JointStates[JointName].JointPosition;
      float DesiredJointPosition = DiffJointPosition / DiffTrajectoryTimeStep * (CurrentTimeStep - LastTimeStep) + LastTrajectoryPoints.JointStates[JointName].JointPosition; 
      float DiffJointVelocity = DesiredTrajectory[TrajectoryPointIndex].JointStates[JointName].JointVelocity - LastTrajectoryPoints.JointStates[JointName].JointVelocity;
      float DesiredJointVelocity = DiffJointVelocity / DiffTrajectoryTimeStep * (CurrentTimeStep - LastTimeStep) + LastTrajectoryPoints.JointStates[JointName].JointVelocity; 
      
      if (GetOwner()->GetJoint(JointName)->GetType()->GetName().Equals("revolute") || GetOwner()->GetJoint(JointName)->GetType()->GetName().Equals("continuous"))
      {
        DesiredJointStates[JointName].JointPosition = FMath::RadiansToDegrees(DesiredJointPosition);
        DesiredJointStates[JointName].JointVelocity = FMath::RadiansToDegrees(DesiredJointVelocity);
      }
      else if (GetOwner()->GetJoint(JointName)->GetType()->GetName().Equals("prismatic"))
      {
        DesiredJointStates[JointName].JointPosition = FConversions::MToCm((float)DesiredJointPosition);
        DesiredJointStates[JointName].JointVelocity = FConversions::MToCm((float)DesiredJointVelocity);
      }
    }
  }
}

bool URJointController::CheckTrajectoryGoalReached()
{
  if (TrajectoryPointIndex == DesiredTrajectory.Num())
  {
    State = UJointControllerState::Normal;
    bPublishResult = true;
    GoalStatusList.Last().Status = 3;
    DesiredTrajectory.Empty();
    TrajectoryPointIndex = 0;
    return true;
  }
  else
  {
    return false;
  }
}

void URJointController::FollowJointTrajectory()
{
  TrajectoryPointIndex = 0;
  LastTrajectoryPoints.Reset();
  URJoint *Joint = nullptr;
  for (const FTrajectoryStatus &TrajectoryStatus : TrajectoryStatusArray)
  {
    Joint = GetOwner()->GetJoint(TrajectoryStatus.JointName);
    if (Joint)
    {
      LastTrajectoryPoints.JointStates.Add(TrajectoryStatus.JointName, Joint->GetJointStateInROSUnit());
    }
  }
  ActionStartTime = GetOwner()->GetGameTimeSinceCreation();
  State = UJointControllerState::FollowJointTrajectory;
}