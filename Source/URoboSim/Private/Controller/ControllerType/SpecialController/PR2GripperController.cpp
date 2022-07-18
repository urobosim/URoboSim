#include "Controller/ControllerType/SpecialController/PR2GripperController.h"

void UPR2GripperController::SetControllerParameters(URControllerParameter *&ControllerParameters)
{
  Super::SetControllerParameters(ControllerParameters);
}

void UPR2GripperController::Init()
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

    if (!GripperJoint)
    {
      UE_LOG(LogTemp, Error, TEXT("GripperJoint of %s not found"), *GetName());
      return;
    }
    GripperJoint->Constraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 1);
    GripperJoint->Constraint->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 1);
    GripperJoint->Constraint->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 1);
    JointValue = GripperJoint->GetJointPosition();
  }
}

void UPR2GripperController::UpdateGripper()
{
  bActive = true;
}

void UPR2GripperController::CheckGripperActionResult(float InError, float InThreshold = 0.5)
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

void UPR2GripperController::Tick(const float &InDeltaTime)
{
  float Error = 0;

  if (!GripperJoint)
  {
    UE_LOG(LogTemp, Error, TEXT("GripperJoint %s of %s not found"),  *GripperJointName, *GetName());
    return;
  }
  if (!GraspComponent)
  {
    UE_LOG(LogTemp, Error, TEXT("GraspComponent of %s not found"), *GetName());
    return;
  }
  GripperPosition = (GripperJoint->GetJointPosition() - PoseOffsetFromJoints);
  // GripperPosition = (GripperJoint->GetJointPosition());
  Error = Position - GripperPosition;


  // UE_LOG(LogTemp, Error, TEXT("GripperJoint: %s GripperPosition %f Error %f Vector %s"), *GripperJoint->GetName(), GripperJoint->GetJointPosition(), Error, *GripperJoint->Constraint->DeltaPoseInJointFrame.ToString());
  // UE_LOG(LogTemp, Error, TEXT("GripperJoint: %s Swing1 %f Swing2 %f Twist %f"), *GripperJoint->GetName(), GripperJoint->Constraint->GetCurrentSwing1(),GripperJoint->Constraint->GetCurrentSwing2(), GripperJoint->Constraint->GetCurrentTwist());
  if (bActive)
  {

    bStalled = false;
    CheckGripperActionResult(Error, 0.001);

    if (GraspComponent->bObjectGrasped && (OldPosition - Position < -0.0012))
    {
      Release();
    }

    if (bActive)
    {
      GoalStatusList.Last().Status = 1;
      ActionDuration += InDeltaTime;

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

UPR2GripperController::UPR2GripperController()
{
  GripperJointName = TEXT("?_gripper_joint");
}
