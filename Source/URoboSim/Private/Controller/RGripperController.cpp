#include "Controller/RGripperController.h"

void URGripperController::UpdateGripper()
{
    bActive = true;
}

void URGripperController::CheckGripperActionResult(float InError, float InThreshold = 0.5)
{
  if(FMath::Abs(InError) < InThreshold)
    {
      Result.FillValues(Position, MaxEffort, false, true);
      GoalStatusList.Last().Status = 3;
      bActive = false;
      bStalled = false;
      bPublishResult = true;
      ActionDuration = 0.0;
    }

  if(ActionDuration > 2)
    {
      ActionDuration = 0.0;
      GoalStatusList.Last().Status = 3;
      bActive = false;
      bPublishResult = true;
      bStalled = true;
      Result.FillValues(GripperPosition, MaxEffort, bStalled, false);
    }
}

void URGripperController::Tick(float InDeltaTime)
{
    float Error = 0;

    if(!RightFinger )
      {
        UE_LOG(LogTemp, Error, TEXT("RightFinger of %s not found"), *GetName());
        return;
      }
    if(!LeftFinger)
      {
        UE_LOG(LogTemp, Error, TEXT("LeftFinger of %s not found"), *GetName());
        return;
      }
    if(!RightFingerTip)
      {

        UE_LOG(LogTemp, Error, TEXT("RightFingerTip of %s not found"), *GetName());
        return;
      }
    if(!LeftFingerTip)
      {
        UE_LOG(LogTemp, Error, TEXT("LeftFingerTip of %s not found"), *GetName());
        return;
      }

    RightFinger->bActuate = true;
    LeftFinger->bActuate = true;

    FVector DirectionTipToTip = (RightFingerTip->Constraint->GetComponentLocation() - LeftFingerTip->Constraint->GetComponentLocation());
    GripperPosition = FMath::Abs((DirectionTipToTip).Size() -3 );
    Error = Position-GripperPosition;
    if(bActive)
    {
      bStalled = false;
      CheckGripperActionResult(Error, 0.5);
      if(bActive)
        {
          GoalStatusList.Last().Status = 1;
          ActionDuration += InDeltaTime;

          if(Error < 0)
            {
              float& RightJointValue = JointController->DesiredJointState.FindOrAdd(RightJointName);
              RightJointValue = RightFinger->GetEncoderValue() - 0.02;
              float& LeftJointValue = JointController->DesiredJointState.FindOrAdd(LeftJointName);
              LeftJointValue = LeftFinger->GetEncoderValue() - 0.02;
            }
          else
            {
              float& RightJointValue = JointController->DesiredJointState.FindOrAdd(RightJointName);
              RightJointValue = RightFinger->GetEncoderValue() + 0.02;
              float& LeftJointValue = JointController->DesiredJointState.FindOrAdd(LeftJointName);
              LeftJointValue = LeftFinger->GetEncoderValue() + 0.02;
            }
        }
      else if (bStalled)
        {
          Grasp();
        }
      else
        {
          Release();
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
  if(GraspComponent)
    {
      return GraspComponent->TryToFixate();
    }
  return false;
}

void URGripperController::Release()
{
  if(GraspComponent)
    {
      GraspComponent->TryToDetach();
    }
}

void URGripperController::Init(ARModel* InModel)
{
  if(!InModel)
    {
      UE_LOG(LogTemp, Error, TEXT("GripperController not attached to ARModel"));
    }
  else
    {

      Model = InModel;
      TArray<URGraspComponent* > TempGraspComponents;
      Model->GetComponents<URGraspComponent>(TempGraspComponents);

      RightFinger = Model->Joints.FindRef(RightJointName);
      LeftFinger = Model->Joints.FindRef(LeftJointName);

      RightFingerTip = Model->Joints.FindRef(RightFingerTipName);
      LeftFingerTip = Model->Joints.FindRef(LeftFingerTipName);

      if(!RightFinger )
        {
          UE_LOG(LogTemp, Error, TEXT("RightFinger of %s not found"), *GetName());
          return;
        }
      if(!LeftFinger)
        {

          UE_LOG(LogTemp, Error, TEXT("LeftFinger of %s not found"), *GetName());
          return;
        }
      if(!RightFingerTip)
        {
          UE_LOG(LogTemp, Error, TEXT("RightFingerTip of %s not found"), *GetName());
          return;
        }
      if(!LeftFingerTip)
        {
          UE_LOG(LogTemp, Error, TEXT("LeftFingerTip of %s not found"), *GetName());
          return;
        }

      ControllerComp = Cast<URControllerComponent>(GetOuter());
      JointController = Cast<URJointController>(ControllerComp->ControllerList("JointController"));

      if(!JointController)
        {
          UE_LOG(LogTemp, Error, TEXT("JointController not found"));
          return;
        }

      for(auto& GraspComp : TempGraspComponents)
        {
          if(GraspComp->GetName().Equals(GraspComponentName))
            {
              GraspComponent = GraspComp;
              URLink* ReferenceLink = Model->Links[GraspCompSetting.GripperName];
              GraspComponent->AttachToComponent(ReferenceLink->GetCollision(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
              GraspComponent->AddRelativeLocation(GraspCompSetting.ToolCenterPoint);
              GraspComponent->Init(ReferenceLink->GetCollision());
            }
        }

      float& RightJointValue = JointController->DesiredJointState.FindOrAdd(RightJointName);
      RightJointValue = RightFinger->GetEncoderValue();
      float& LeftJointValue = JointController->DesiredJointState.FindOrAdd(LeftJointName);
      LeftJointValue = LeftFinger->GetEncoderValue();
    }
}
