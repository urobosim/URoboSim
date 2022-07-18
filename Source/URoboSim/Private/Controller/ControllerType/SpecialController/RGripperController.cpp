#include "Controller/ControllerType/SpecialController/RGripperController.h"

void URGripperController::SetControllerParameters(URControllerParameter *&ControllerParameters)
{
  Super::SetControllerParameters(ControllerParameters);
}

void URGripperController::Init()
{
  Super::Init();

  if (!GetOwner())
  {
    UE_LOG(LogTemp, Error, TEXT("%s not attached to ARModel"), *GetName());
  }
  else
  {
    PoseOffsetFromJoints = GripperJoint->GetJointPosition();

    if (!JointController)
    {
      UE_LOG(LogTemp, Error, TEXT("JointController not found"));
      return;
    }

    TArray<FString> JointNames;
    JointNames.Add(GripperJointName);
    // JointController->SetJointNames(JointNames, EnableDrive);
    JointController->SetJointNames(JointNames);

    if (!GripperJoint)
    {
      UE_LOG(LogTemp, Error, TEXT("RightFinger of %s not found"), *GetName());
      return;
    }
    JointValue = GripperJoint->GetJointPosition();
  }
}

void URGripperController::UpdateGripper()
{
  bActive = true;
}

void URGripperController::CheckGripperActionResult(float InError, float InThreshold = 0.5)
{
  if (FMath::Abs(InError) < InThreshold)
  {
    Result.FillValues(Position, MaxEffort, false, true);
    GoalStatusList.Last().Status = 3;
    bActive = false;
    bStalled = false;
    bPublishResult = true;
    ActionDuration = 0.0;
  }

  if (ActionDuration > 2)
  {
    ActionDuration = 0.0;
    GoalStatusList.Last().Status = 3;
    bActive = false;
    bPublishResult = true;
    bStalled = true;
    Result.FillValues(GripperPosition, MaxEffort, bStalled, false);
  }
}

void URGripperController::Tick(const float &InDeltaTime)
{
  float Error = 0;

  if (!GripperJoint)
  {
    UE_LOG(LogTemp, Error, TEXT("GripperJoint of %s not found"), *GetName());
    return;
  }
  GripperPosition = (GripperJoint->GetJointPosition() - PoseOffsetFromJoints);
  Error = Position - GripperPosition;


  // UE_LOG(LogTemp, Error, TEXT("GripperJoint: %s GripperPosition %f Error %f"), *GripperJoint->GetName(), GripperPosition, Error);
  // UE_LOG(LogTemp, Error, TEXT("GripperJoint: %s Swing1 %f Swing2 %f Twist %f"), *GripperJoint->GetName(), GripperJoint->Constraint->GetCurrentSwing1(),GripperJoint->Constraint->GetCurrentSwing2(), GripperJoint->Constraint->GetCurrentTwist());
  if (bActive)
  {

    bStalled = false;
    CheckGripperActionResult(Error, 0.0005);

    if (GraspComponent->bObjectGrasped && (OldPosition - Position < -0.0012))
    {
      Release();
    }

    if (bActive)
    {
      GoalStatusList.Last().Status = 1;
      ActionDuration += InDeltaTime;

      UE_LOG(LogTemp, Error, TEXT("GripperJoint: %s GripperPosition %f Error %f"), *GripperJoint->GetName(), GripperPosition, Error);
      float &GripperJointValue = JointController->DesiredJointStates.FindOrAdd(GripperJointName).JointPosition;
      GripperJointValue = Position;

    }
    else if (bStalled && (OldPosition - Position >= -0.0012))
    {
      float &GripperJointValue = JointController->DesiredJointStates.FindOrAdd(GripperJointName).JointPosition;
      OldPosition = GripperPosition;
      Grasp();
    }
    else
    {
      OldPosition = GripperPosition;
    }
  }
}

URGripperController::URGripperController()
{
  GripperJointName = TEXT("?_gripper_joint");
}
