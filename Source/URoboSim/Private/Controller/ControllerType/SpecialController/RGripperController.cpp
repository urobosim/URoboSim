#include "Controller/ControllerType/SpecialController/RGripperController.h"

void URGripperController::SetControllerParameters(URControllerParameter *&ControllerParameters)
{
  URGripperControllerParameter *GripperControllerParameters = Cast<URGripperControllerParameter>(ControllerParameters);
  if (GripperControllerParameters)
  {
    RightJointName = GripperControllerParameters->RightJointName;
    LeftJointName = GripperControllerParameters->LeftJointName;
    RightFingerTipName = GripperControllerParameters->RightFingerTipName;
    LeftFingerTipName = GripperControllerParameters->LeftFingerTipName;
    GraspCompSetting = GripperControllerParameters->GraspCompSetting;
    EnableDrive = GripperControllerParameters->EnableDrive;
    bDisableCollision = GripperControllerParameters->bDisableCollision;
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

    RightFinger = GetOwner()->Joints.FindRef(RightJointName);
    LeftFinger = GetOwner()->Joints.FindRef(LeftJointName);

    RightFingerTip = GetOwner()->Joints.FindRef(RightFingerTipName);
    LeftFingerTip = GetOwner()->Joints.FindRef(LeftFingerTipName);

    if (!RightFinger)
    {
      UE_LOG(LogTemp, Error, TEXT("RightFinger of %s not found"), *GetName());
      return;
    }
    if (!LeftFinger)
    {

      UE_LOG(LogTemp, Error, TEXT("LeftFinger of %s not found"), *GetName());
      return;
    }
    if (!RightFingerTip)
    {
      UE_LOG(LogTemp, Error, TEXT("RightFingerTip of %s not found"), *GetName());
      return;
    }
    if (!LeftFingerTip)
    {
      UE_LOG(LogTemp, Error, TEXT("LeftFingerTip of %s not found"), *GetName());
      return;
    }

    if (bDisableCollision)
    {
      RightFinger->Child->DisableCollision();
      LeftFinger->Child->DisableCollision();
      RightFingerTip->Child->DisableCollision();
      LeftFingerTip->Child->DisableCollision();
    }

    PoseOffsetFromJoints = (RightFingerTip->Constraint->GetComponentLocation() - LeftFingerTip->Constraint->GetComponentLocation()).Size();

    JointController = Cast<URJointController>(GetOwner()->GetController(TEXT("JointController")));

    if (!JointController)
    {
      UE_LOG(LogTemp, Error, TEXT("JointController not found"));
      return;
    }
    TArray<FString> JointNames;
    JointNames.Add(RightJointName);
    JointNames.Add(LeftJointName);
    JointController->SetJointNames(JointNames, EnableDrive);

    for (auto &GraspComp : TempGraspComponents)
    {
      if (GraspComp->GetName().Equals(GraspComponentName))
      {
        GraspComponent = GraspComp;
        URLink *ReferenceLink = GetOwner()->Links[GraspCompSetting.GripperName];
        GraspComponent->AttachToComponent(ReferenceLink->GetCollision(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        GraspComponent->AddRelativeLocation(GraspCompSetting.ToolCenterPoint);

        if (bUseMultipleConstraints)
        {
          GraspComponent->Init(RightFinger->Child->GetCollision(), LeftFinger->Child->GetCollision());
        }
        else
        {
          GraspComponent->Init(ReferenceLink->GetCollision());
        }
      }
    }

    float &RightJointValue = JointController->DesiredJointStates.FindOrAdd(RightJointName).JointPosition;
    RightJointValue = RightFinger->GetEncoderValue();
    float &LeftJointValue = JointController->DesiredJointStates.FindOrAdd(LeftJointName).JointPosition;
    LeftJointValue = LeftFinger->GetEncoderValue();
    JointValue = (RightJointValue + LeftJointValue) / 2.0;
  }
}

void URGripperController::UpdateGripper()
{
  SetGripperCollision(true);
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

  if (!RightFinger)
  {
    UE_LOG(LogTemp, Error, TEXT("RightFinger of %s not found"), *GetName());
    return;
  }
  if (!LeftFinger)
  {
    UE_LOG(LogTemp, Error, TEXT("LeftFinger of %s not found"), *GetName());
    return;
  }
  if (!RightFingerTip)
  {

    UE_LOG(LogTemp, Error, TEXT("RightFingerTip of %s not found"), *GetName());
    return;
  }
  if (!LeftFingerTip)
  {
    UE_LOG(LogTemp, Error, TEXT("LeftFingerTip of %s not found"), *GetName());
    return;
  }

  FVector DirectionTipToTip = (RightFingerTip->Constraint->GetComponentLocation() - LeftFingerTip->Constraint->GetComponentLocation());
  GripperPosition = FMath::Abs((DirectionTipToTip).Size() - PoseOffsetFromJoints);
  Error = Position - GripperPosition;

  if (bActive)
  {

    bStalled = false;
    CheckGripperActionResult(Error, 0.1);

    if (GraspComponent->bObjectGrasped && (OldPosition - Position < -0.12))
    {
      UE_LOG(LogTemp, Error, TEXT("Release: OldPosition %f NewPosition %f"), OldPosition, Position);
      Release();
      RightFinger->bActuate = true;
      LeftFinger->bActuate = true;
    }

    if (bActive)
    {
      RightFinger->bActuate = true;
      LeftFinger->bActuate = true;
      GoalStatusList.Last().Status = 1;
      ActionDuration += InDeltaTime;

      JointValue = (LeftFinger->GetEncoderValue() + RightFinger->GetEncoderValue()) / 2.0;

      // The larger the error the faster the gripper should move. Close to the goal it should be slow to avoid oscillation
      float Speed = 0.02 + GripperSpeedFactor * (FMath::Abs(Error) / 6.5);
      float &RightJointValue = JointController->DesiredJointStates.FindOrAdd(RightJointName).JointPosition;
      float &LeftJointValue = JointController->DesiredJointStates.FindOrAdd(LeftJointName).JointPosition;
      if (Error < 0)
      {
        RightJointValue -= Speed;
        LeftJointValue -= Speed;
      }
      else
      {
        RightJointValue += Speed;
        LeftJointValue += Speed;
      }
      // RightJointValue = RightFinger->ClampJointStateToConstraintLimit(RightJointValue);
      // LeftJointValue = LeftFinger->ClampJointStateToConstraintLimit(LeftJointValue);
    }
    else if (bStalled && (OldPosition - Position >= -0.12))
    {
      RightFinger->bActuate = true;
      LeftFinger->bActuate = true;

      float &RightJointValue = JointController->DesiredJointStates.FindOrAdd(RightJointName).JointPosition;
      float &LeftJointValue = JointController->DesiredJointStates.FindOrAdd(LeftJointName).JointPosition;
      RightJointValue = RightFinger->GetEncoderValue();
      LeftJointValue = LeftFinger->GetEncoderValue();

      OldPosition = GripperPosition;
      UE_LOG(LogTemp, Error, TEXT("Grasp: OldPosition %f NewPosition %f"), OldPosition, Position);
      Grasp();
    }
    else
    {
      RightFinger->bActuate = true;
      LeftFinger->bActuate = true;
      OldPosition = GripperPosition;
      UE_LOG(LogTemp, Error, TEXT("GripperError %f"), Error);
      // OldPosition = GripperPosition;
      // UE_LOG(LogTemp, Error, TEXT("Release"));
      // Release();
    }
  }
}

URGripperController::URGripperController()
{
  RightFingerTipName = TEXT("?_gripper_r_finger_tip_joint");
  RightJointName = TEXT("?_gripper_r_finger_joint");
  LeftFingerTipName = TEXT("?_gripper_l_finger_tip_joint");
  LeftJointName = TEXT("?_gripper_l_finger_joint");
}

bool URGripperController::Grasp()
{
  if (GraspComponent)
  {
    SetGripperCollision(false);
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

void URGripperController::SetGripperCollision(bool InCollisionEnabled)
{
  if (InCollisionEnabled)
  {
    RightFinger->Child->EnableCollision();
    LeftFinger->Child->EnableCollision();
    RightFingerTip->Child->EnableCollision();
    LeftFingerTip->Child->EnableCollision();
  }
  else
  {
    RightFinger->Child->DisableCollision();
    LeftFinger->Child->DisableCollision();
    RightFingerTip->Child->DisableCollision();
    LeftFingerTip->Child->DisableCollision();
  }
}
