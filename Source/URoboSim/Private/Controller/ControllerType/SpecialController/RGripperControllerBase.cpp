#include "Controller/ControllerType/SpecialController/RGripperControllerBase.h"
#include "URoboSimSettings.h"

URGripperControllerBase::URGripperControllerBase()
{
  GripperJointNames.Empty();
	GripperJointNames.Add(TEXT("?_gripper_joint"));
        InitPriority = 2;
	// GraspComponent = CreateDefaultSubobject<URGraspComponent>(FName(GetName() + TEXT("_GraspComp")));
	// GraspComponent->RegisterComponent();
	// if (GetOwner())
	// {
	// 	GraspComponent->AttachToComponent(GetOwner()->GetRootComponent(),
	// 	                                  FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("%s not attached to ARModel"), *GetName());
	// }
}

void URGripperControllerBase::SetControllerParameters(URControllerParameter*& ControllerParameters)
{
	URGripperControllerBaseParameter* GripperControllerParameters = Cast<URGripperControllerBaseParameter>(
		ControllerParameters);
	if (GripperControllerParameters)
	{
		GripperJointNames = GripperControllerParameters->GripperJointNames;
		GraspCompSetting = GripperControllerParameters->GraspCompSetting;
		GraspComponentName = GripperControllerParameters->GraspComponentName;
		EnableDrive = GripperControllerParameters->EnableDrive;
		Mode = GripperControllerParameters->Mode;
		bOverwriteConfig = GripperControllerParameters->bOverwriteConfig;
		bInvertGraspCondition = GripperControllerParameters->bInvertGraspCondition;
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

          const UURoboSimSettings* Settings = GetDefault<UURoboSimSettings>();
          if(Settings)
            {
              bDebugMode = Settings->bDebugMode;
            }

          for(auto & GripperJointName: GripperJointNames)
            {
              URJoint* GripperJoint = GetOwner()->Joints.FindRef(GripperJointName);

              if (!GripperJoint)
		{
                  UE_LOG(LogTemp, Error, TEXT("GripperJoint %s of %s not found"), *GetName(), *GripperJointName);
                  return;
		}
              else
		{
                  UE_LOG(LogTemp, Log, TEXT("GripperJoint %s of %s found"), *GetName(), *GripperJointName);
		}
              GripperJoints.Add(GripperJoint);
              OldPositions.Add(GripperJointName,0);
            }

          JointController = Cast<URJointController>(GetOwner()->GetController(TEXT("JointController")));

		if (!JointController)
		{
			UE_LOG(LogTemp, Error, TEXT("%s: JointController not found"), *GetName());
			return;
		}

		TArray<FString> JointNames;
		JointNames.Append(GripperJointNames);
		JointController->SetJointNames(JointNames, EnableDrive);
		if (bOverwriteConfig)
		{
                  for(auto & GripperJoint: GripperJoints)
                    {
			JointController->AddConfigOverwrite(GripperJoint->GetName(), FConfigOverwrite(Mode, EnableDrive));
                    }
		}

		// OldPosition = JointController->DesiredJointStates.FindRef(GripperJointName).JointPosition;

		GraspComponent = NewObject<URGraspComponent>(GetOwner(), FName(GetName() + TEXT("_GraspComp")));
		GraspComponent->CreationMethod = EComponentCreationMethod::Instance;
		GraspComponent->RegisterComponent();

		ReferenceLink = GetOwner()->Links[GraspCompSetting.GripperName];
		GraspComponent->AttachToComponent(ReferenceLink, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		GraspComponent->Constraint->AttachToComponent(GraspComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		GraspComponent->AddRelativeLocation(GraspCompSetting.ToolCenterPoint);
                GraspComponent->GraspRadius = GraspCompSetting.Radius;
		GraspComponent->Init(ReferenceLink);
 	}
}

void URGripperControllerBase::Tick(const float& InDeltaTime)
{
  float Diff = 0;
  float JointPos = 0;
  float OldPosition = 0;
  for(auto& GripperJoint : GripperJoints)
    {
      // if (GripperJoint)
      //   {
      //     UE_LOG(LogTemp, Error, TEXT("GripperJoint: %s of %s not set"), *GetName(), *GripperJointNames[0]);
      //     return;
      //   }
      float TempPos = JointController->DesiredJointStates.FindRef(GripperJoint->GetName()).JointPosition;

      float JointDiff = FMath::Abs(TempPos - OldPositions[GripperJoint->GetName()]);
      if(JointDiff > Diff)
        {
          Diff = JointDiff;
          JointPos = TempPos;
          OldPosition = OldPositions[GripperJoint->GetName()];
        }
      if(bDebugMode)
        {
          UE_LOG(LogTemp, Log, TEXT("%s: Diff %f   %f      %f"), *GripperJoint->GetName(), JointDiff, TempPos, OldPositions[GripperJoint->GetName()]);
        }
      OldPositions[GripperJoint->GetName()] = TempPos;
    }

  if (Diff > 0.01)
	{
		if (JointPos < OldPosition)
		{
			if (bInvertGraspCondition)
			{
				Release();
			}
			else
			{
				Grasp();
			}
		}
		else if (JointPos > OldPosition)
		{
			if (bInvertGraspCondition)
			{
				Grasp();
			}
			else
			{
				Release();
			}
		}
	}
	// OldPosition = JointPos;
}

bool URGripperControllerBase::Grasp()
{
  if(bDebugMode)
    {
      UE_LOG(LogTemp, Log, TEXT("%s: Grasp"), *GetName());
    }
	if (GraspComponent)
	{
		return GraspComponent->TryToFixate();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%s: No Grasp Component"), *GetName());
	}
	return false;
}

void URGripperControllerBase::Release()
{
  if(bDebugMode)
    {
      UE_LOG(LogTemp, Log, TEXT("%s: Release"), *GetName());
    }

	if (GraspComponent)
	{
		GraspComponent->TryToDetach();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("%s: No Grasp Component"), *GetName());
	}
}
