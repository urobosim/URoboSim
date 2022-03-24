#include "Controller/ControllerType/SpecialController/RGripperControllerBase.h"

void URGripperControllerBase::SetControllerParameters(URControllerParameter *&ControllerParameters)
{
  URGripperControllerBaseParameter *GripperControllerParameters = Cast<URGripperControllerBaseParameter>(ControllerParameters);
  if (GripperControllerParameters)
  {
    GripperJointName = GripperControllerParameters->GripperJointName;
    GraspCompSetting = GripperControllerParameters->GraspCompSetting;
    GraspComponentName = GripperControllerParameters->GraspComponentName;
    EnableDrive = GripperControllerParameters->EnableDrive;
    Mode = GripperControllerParameters->Mode;
    bOverwriteConfig = GripperControllerParameters->bOverwriteConfig;
  }
}

void URGripperControllerBase::Init()
{
  Super::Init();

  if (!GetOwner())
  {
    UE_LOG(LogTemp, Error, TEXT("%s not attached to ARModel"), *GetName());
  }
  else
  {

    GripperJoint = GetOwner()->Joints.FindRef(GripperJointName);

    if (!GripperJoint)
    {
      UE_LOG(LogTemp, Error, TEXT("RightFinger of %s not found"), *GetName());
      return;
    }

    JointController = Cast<URJointController>(GetOwner()->GetController(TEXT("JointController")));

    if (!JointController)
    {
      UE_LOG(LogTemp, Error, TEXT("%s: JointController not found"), *GetName());
      return;
    }

    if(bOverwriteConfig)
      {

        JointController->AddConfigOverwrite(GripperJoint->GetName(), FConfigOverwrite(Mode, EnableDrive));

        TArray<FString> JointNames;
        JointNames.Add(GripperJointName);
        JointController->SetJointNames(JointNames, EnableDrive);
      }

    OldPosition = JointController->DesiredJointStates.FindRef(GripperJointName).JointPosition;

    TArray<URGraspComponent *> TempGraspComponents;
    GetOwner()->GetComponents<URGraspComponent>(TempGraspComponents);

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
  }
}

void URGripperControllerBase::Tick(const float &InDeltaTime)
{
  if (!GripperJoint)
  {
    UE_LOG(LogTemp, Error, TEXT("GripperJoint of %s not found"), *GetName());
    return;
  }
  float JointPos = JointController->DesiredJointStates.FindRef(GripperJointName).JointPosition;

  if(FMath::Abs(JointPos - OldPosition) > 0.01)
    {
      if(JointPos < OldPosition)
        {
          if(bInvertGraspCondition)
            {
              Release();
            }
          else
            {
              Grasp();
            }
        }
      else if(JointPos > OldPosition)
        {
          if(bInvertGraspCondition)
            {
              Grasp();
            }
          else
            {
              Release();
            }

        }
    }
  OldPosition = JointPos;
}

URGripperControllerBase::URGripperControllerBase()
{
  GripperJointName = TEXT("?_gripper_joint");
}

bool URGripperControllerBase::Grasp()
{
  if (GraspComponent)
  {
    return GraspComponent->TryToFixate();
  }
  else
    {

      UE_LOG(LogTemp, Error, TEXT("%s: No Grasp Component"), *GetName());
    }
  return false;
}

void URGripperControllerBase::Release()
{
  if (GraspComponent)
  {
    GraspComponent->TryToDetach();
  }
  else
    {

      UE_LOG(LogTemp, Error, TEXT("%s: No Grasp Component"), *GetName());
    }
}
