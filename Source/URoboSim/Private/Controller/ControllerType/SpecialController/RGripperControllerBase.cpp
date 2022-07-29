#include "Controller/ControllerType/SpecialController/RGripperControllerBase.h"

URGripperControllerBase::URGripperControllerBase()
{
	GripperJointName = TEXT("?_gripper_joint");
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
		GripperJointName = GripperControllerParameters->GripperJointName;
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
		GripperJoint = GetOwner()->Joints.FindRef(GripperJointName);

		if (!GripperJoint)
		{
			UE_LOG(LogTemp, Error, TEXT("GripperJoint %s of %s not found"), *GetName(), *GripperJointName);
			return;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("GripperJoint %s of %s found"), *GetName(), *GripperJointName);
		}

		JointController = Cast<URJointController>(GetOwner()->GetController(TEXT("JointController")));

		if (!JointController)
		{
			UE_LOG(LogTemp, Error, TEXT("%s: JointController not found"), *GetName());
			return;
		}

		TArray<FString> JointNames;
		JointNames.Add(GripperJointName);
		JointController->SetJointNames(JointNames, EnableDrive);
		if (bOverwriteConfig)
		{
			JointController->AddConfigOverwrite(GripperJoint->GetName(), FConfigOverwrite(Mode, EnableDrive));
		}

		// OldPosition = JointController->DesiredJointStates.FindRef(GripperJointName).JointPosition;
		OldPosition = 0;

		GraspComponent = NewObject<URGraspComponent>(GetOwner(), FName(GetName() + TEXT("_GraspComp")));
		GraspComponent->CreationMethod = EComponentCreationMethod::Instance;
		GraspComponent->RegisterComponent();
		
		URLink* ReferenceLink = GetOwner()->Links[GraspCompSetting.GripperName];
		GraspComponent->AttachToComponent(ReferenceLink, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		GraspComponent->Constraint->AttachToComponent(GraspComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		GraspComponent->AddRelativeLocation(GraspCompSetting.ToolCenterPoint);
		GraspComponent->Init(ReferenceLink);
 	}
}

void URGripperControllerBase::Tick(const float& InDeltaTime)
{
	if (!GripperJoint)
	{
		UE_LOG(LogTemp, Error, TEXT("GripperJoint: %s of %s not set"), *GetName(), *GripperJointName);
		return;
	}
	float JointPos = JointController->DesiredJointStates.FindRef(GripperJointName).JointPosition;

	if (FMath::Abs(JointPos - OldPosition) > 0.01)
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
	OldPosition = JointPos;
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
