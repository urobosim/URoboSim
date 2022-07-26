#include "Factory/RModelBuilder.h"
#include "Controller/RControllerComponent.h"
#include "Controller/ControllerType/BaseController/RBaseController.h"


// Sets default values
URModelBuilder::URModelBuilder()
{
	LinkFactory = CreateDefaultSubobject<URLinkFactory>(TEXT("LinkFactory"));
	JointFactory = CreateDefaultSubobject<URJointFactory>(TEXT("JointFactory"));
}

// Dtor
URModelBuilder::~URModelBuilder()
{
}

// Load model
void URModelBuilder::Load(USDFModel* InModelDescription, ARModel* OutModel, FVector InLocation, FString InVersion)
{
	ModelDescription = InModelDescription;
	Version = InVersion;
	LoadLocation = InLocation;
	if (OutModel)
	{
		Model = OutModel;
		LoadLinks(InLocation);
		LoadJoints();
		BuildKinematicTree();
		SetupPlugins();
	}
}

// Load links
void URModelBuilder::LoadLinks(FVector InLocation)
{
	for (USDFLink* Link : ModelDescription->Links)
	{
		URLink* TempLink = LinkFactory->Load(Model, Link, InLocation, Version);
		if (TempLink)
		{
			if (!Model->BaseLink)
			{
				Model->BaseLink = TempLink;
			}
			Model->AddLink(TempLink);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Creation of Link %s failed"), *Link->GetName());
		}
	}
}

// Load joints
void URModelBuilder::LoadJoints()
{
	for (USDFJoint* Joint : ModelDescription->Joints)
	{
		URLink* Parent = Model->GetLink(Joint->Parent);
		if(!Parent)
		{
			continue;
		}
		
		// if(!Parent->GetCollision())
		// {
		// 	UE_LOG(LogTemp, Error, TEXT("Joint %s Parent %s has no collision. Not supported"), *Joint->GetName(), *Joint->Parent);
		// 	continue;
		// }
		
		URLink* Child = Model->GetLink(Joint->Child);
		if(!Child)
		{
			continue;
		}
		
		// if (!Child->GetCollision())
		// {
		// 	UE_LOG(LogTemp, Error, TEXT("Joint %s Child %s has no collision. Not supported"), *Joint->GetName(), *Joint->Child);
		// 	continue;
		// }

		
		URJoint* TempJoint = JointFactory->Load(Model, Joint, Version);
		if (TempJoint)
		{
			Model->AddJoint(TempJoint);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Creation of Joint %s failed"), *Joint->GetName());
		}
	}
}

void URModelBuilder::BuildKinematicTree()
{
	if (!Model)
	{
		UE_LOG(LogTemp, Error, TEXT("Model does not exist"));
		return;
	}

	for (auto& Joint : Model->Joints)
	{
		URLink* Parent = *Model->Links.Find(Joint.Value->ParentName);
		URLink* Child = *Model->Links.Find(Joint.Value->ChildName);
		if (!Parent)
		{
			UE_LOG(LogTemp, Error, TEXT("Joint %s Parent %s not found"), *Joint.Value->GetName(), *Joint.Value->ParentName);
			continue;
		}
		
		if (!Child)
		{
			UE_LOG(LogTemp, Error, TEXT("Joint %s Child %s not found"), *Joint.Value->GetName(), *Joint.Value->ChildName);
			continue;
		}
		
		if (!Child->bAttachedToParent)
		{
			Child->AttachToComponent(Parent, FAttachmentTransformRules::KeepWorldTransform);
			Child->bAttachedToParent = true;
		}
		Joint.Value->SetParentChild(Parent, Child);
		SetConstraintPosition(Joint.Value);

		if (Child->PoseRelativeTo.Equals(TEXT("Default")))
		{
			Child->SetWorldTransform(Child->LinkPoseInternal);
		}
		else
		{
			URJoint* RefJoint = *Model->Joints.Find(Child->PoseRelativeTo);
			if (RefJoint)
			{
				Child->SetWorldTransform(RefJoint->GetComponentTransform());
				Child->AddLocalTransform(Child->LinkPoseInternal);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("RefJoint %s of %s not found"), *Child->PoseRelativeTo, *Child->GetName());
			}
		}


		Joint.Value->Constraint->ConnectToComponents();

		Parent->AddJoint(Joint.Value);
	}
}

void URModelBuilder::SetConstraintPosition(URJoint* InJoint)
{
	if (InJoint)
	{
		if (!InJoint->Parent)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] Parent of Joint %s not valid"), *FString(__FUNCTION__),
			       *InJoint->GetName());
			return;
		}
		
		if (!InJoint->Child)
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] Child of Joint %s not valid"), *FString(__FUNCTION__),
			       *InJoint->GetName());
			return;
		}

		if (InJoint->PoseRelativeTo.Equals(TEXT("Default")))
		{
			if (InJoint->bUseParentModelFrame)
			{
				// InJoint->AttachToComponent(InJoint->Child->GetCollision(), FAttachmentTransformRules::KeepWorldTransform);
				InJoint->SetWorldTransform(InJoint->Parent->GetComponentTransform());
				InJoint->AddLocalTransform(InJoint->Pose);

				InJoint->AttachToComponent(InJoint->Parent, FAttachmentTransformRules::KeepWorldTransform);

				UE_LOG(LogTemp, Warning, TEXT("Souldn't be used with 1.7"));
			}
			else
			{
				// InJoint->Constraint->SetPosition(InJoint);
				InJoint->AttachToComponent(InJoint->Child, FAttachmentTransformRules::KeepRelativeTransform);
				if(InJoint->Parent->PoseRelativeTo.Equals(TEXT("Default")))
				{
					InJoint->Parent->SetWorldTransform(Model->GetRootComponent()->GetComponentTransform());
					InJoint->Parent->AddLocalTransform(InJoint->Parent->LinkPoseInternal);
				}
			}
		}
		else
		{
			URLink* RefLink = *Model->Links.Find(InJoint->PoseRelativeTo);
			if (IsValid(RefLink))
			{
				FTransform TempTransform = RefLink->GetComponentTransform();
				InJoint->SetWorldTransform(TempTransform);
				InJoint->AddLocalTransform(InJoint->Pose);
				InJoint->AttachToComponent(InJoint->Parent, FAttachmentTransformRules::KeepWorldTransform);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("RefLink not Valid for Joint %s"), *InJoint->GetName());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Joint not valid"), *FString(__FUNCTION__));
	}
}

void URModelBuilder::SetupPlugins()
{
	for (USDFPlugin* Plugin : ModelDescription->Plugins)
	{
		URJoint* Joint = Model->Joints[Plugin->Joint];

		if (Joint)
		{
			URJoint* MimicJoint = Model->Joints[Plugin->MimicJoint];
			if (MimicJoint)
			{
				MimicJoint->bIsMimic = true;
				FMimicJointParameter MimicJointParameter;
				MimicJointParameter.MimicJoint = MimicJoint;
				MimicJointParameter.Multiplier = Plugin->Multiplier;
				Joint->MimicJointList.Add(MimicJointParameter);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("MimicJoint %s not found"), *Plugin->MimicJoint);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Joint  to Mimic %s not found"), *Plugin->Joint);
		}
	}
}
