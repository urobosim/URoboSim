#include "Controller/ControllerType/SpecialController/RGripperController.h"

void URGripperController::SetControllerParameters(URControllerParameter *&ControllerParameters)
{
  URGripperControllerParameter *GripperControllerParameters = Cast<URGripperControllerParameter>(ControllerParameters);
  if (GripperControllerParameters)
  {
    GripperJointName = GripperControllerParameters->GripperJointName;
    GraspCompSetting = GripperControllerParameters->GraspCompSetting;
    EnableDrive = GripperControllerParameters->EnableDrive;
  }
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
    TArray<URGraspComponent *> TempGraspComponents;
    GetOwner()->GetComponents<URGraspComponent>(TempGraspComponents);

    GripperJoint = GetOwner()->Joints.FindRef(GripperJointName);

    if (!GripperJoint)
    {
      UE_LOG(LogTemp, Error, TEXT("RightFinger of %s not found"), *GetName());
      return;
    }

    PoseOffsetFromJoints = GripperJoint->GetJointPositionInUUnits();

    UE_LOG(LogTemp, Error, TEXT("Offset %f"), PoseOffsetFromJoints);
    JointController = Cast<URJointController>(GetOwner()->GetController(TEXT("JointController")));

    if (!JointController)
    {
      UE_LOG(LogTemp, Error, TEXT("JointController not found"));
      return;
    }

    TArray<FString> JointNames;
    JointNames.Add(GripperJointName);
    JointController->SetJointNames(JointNames, EnableDrive);

    for (auto &GraspComp : TempGraspComponents)
    {
      if (GraspComp->GetName().Equals(GraspComponentName))
      {
        GraspComponent = GraspComp;
        URLink *ReferenceLink = GetOwner()->Links[GraspCompSetting.GripperName];
        GraspComponent->AttachToComponent(ReferenceLink->GetCollision(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        GraspComponent->AddRelativeLocation(GraspCompSetting.ToolCenterPoint);
        GraspComponent->Init(ReferenceLink->GetCollision());
      }
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
  GripperPosition = (GripperJoint->GetJointPositionInUUnits() - PoseOffsetFromJoints);
  Error = Position - GripperPosition;

  if (bActive)
  {

    bStalled = false;
    CheckGripperActionResult(Error, 0.1);

    if (GraspComponent->bObjectGrasped && (OldPosition - Position < -0.12))
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
    else if (bStalled && (OldPosition - Position >= -0.12))
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

bool URGripperController::Grasp()
{
  if (GraspComponent)
  {
    return GraspComponent->TryToFixate();
  }
  return false;
}

void URGripperController::Release()
{
  if (GraspComponent)
  {
    GraspComponent->TryToDetach();
  }
}
