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

		GripperJoint2 = GetOwner()->Joints.FindRef(GripperJointName2);
		GripperJoint3 = GetOwner()->Joints.FindRef(GripperJointName3);

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
    
		if (!GripperJoint2)
		{
			UE_LOG(LogTemp, Error, TEXT("GripperJoint %s of %s not found"), *GetName(), *GripperJointName2);
			return;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GripperJoint %s of %s found"), *GetName(), *GripperJointName2);
		}
		if (!GripperJoint3)
		{
			UE_LOG(LogTemp, Error, TEXT("GripperJoint %s of %s not found"), *GetName(), *GripperJointName3);
			return;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GripperJoint %s of %s found"), *GetName(), *GripperJointName3);
		}
    
    PoseOffsetFromJoints = GripperJoint3->GetJointPosition();
    
		TArray<FString> JointNames;
		JointNames.Add(GripperJointName2);
		JointController->SetJointNames(JointNames, EnableDrive);
    
    GripperJoint3->Constraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 1);
    GripperJoint3->Constraint->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Limited, 1);
    GripperJoint3->Constraint->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Limited, 1);
    GripperJoint3->Constraint->ConstraintInstance.SetLinearXLimit(ELinearConstraintMotion::LCM_Limited, 1.0);
    GripperJoint3->Constraint->ConstraintInstance.SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 1.0);
    
    JointValue = GripperJoint3->GetJointPosition();
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
  if (!GripperJoint2)
  {
    UE_LOG(LogTemp, Error, TEXT("GripperJoint2 %s of %s not found"),  *GripperJointName2, *GetName());
    return;
  }
  if (!GripperJoint3)
  {
    UE_LOG(LogTemp, Error, TEXT("GripperJoint3 %s of %s not found"),  *GripperJointName3, *GetName());
    return;
  }
  if (!GraspComponent)
  {
    UE_LOG(LogTemp, Error, TEXT("GraspComponent of %s not found"), *GetName());
    return;
  }
  // GripperPosition = (GripperJoint3->GetJointPosition() - PoseOffsetFromJoints);
  GripperPosition = (GripperJoint3->GetJointPosition());
  Error = Position - GripperPosition;


  // UE_LOG(LogTemp, Error, TEXT("GripperJoint: %s Swing1 %f Swing2 %f Twist %f"), *GripperJoint->GetName(), GripperJoint->Constraint->GetCurrentSwing1(),GripperJoint->Constraint->GetCurrentSwing2(), GripperJoint->Constraint->GetCurrentTwist());
  if (bActive)
  {
    UE_LOG(LogTemp, Error, TEXT("GripperJoint: %s GripperPosition %f Error %f PoseOffsetFromJoints %f"), *GripperJoint3->GetName(), GripperJoint3->GetJointPosition(), Error, PoseOffsetFromJoints);

    bStalled = false;
    CheckGripperActionResult(Error, 0.001);

    if (GraspComponent->bObjectGrasped && (OldPosition - Position < -0.0012))
    {
      UE_LOG(LogTemp, Error, TEXT("%s: Release"), *GraspComponent->GetName());
      Release();
    }

    if (bActive)
    {
      GoalStatusList.Last().Status = 1;
      ActionDuration += InDeltaTime;
      float Speed = 0.01;// + 0.01 * (FMath::Abs(Error) / 0.085);
      float &GripperJointValue = JointController->DesiredJointStates.FindOrAdd(GripperJointName).JointPosition;
      float &GripperJointValue2= JointController->DesiredJointStates.FindOrAdd(GripperJointName2).JointPosition;
      if (Error < 0.)
       {
         GripperJointValue -= Speed;
         GripperJointValue2 -= Speed;
       }
       else if(Error > 0.)
       {
         GripperJointValue += Speed;
         GripperJointValue2 += Speed;
       }     
      
      // GripperJointValue = Position;

    }
    else if (bStalled && (OldPosition - Position >= -0.0012))
    {
      float &GripperJointValue = JointController->DesiredJointStates.FindOrAdd(GripperJointName).JointPosition;
      float &GripperJointValue2 = JointController->DesiredJointStates.FindOrAdd(GripperJointName2).JointPosition;
      float Average = (GripperJoint->GetJointPosition() + GripperJoint2->GetJointPosition()) / 2.0;
      GripperJointValue = Average;
      GripperJointValue2 = Average;
      
      OldPosition = GripperPosition;
      UE_LOG(LogTemp, Error, TEXT("%s: Grasp"), *GraspComponent->GetName());
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
