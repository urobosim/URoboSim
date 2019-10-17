#include "Factory/RModelBuilder.h"


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
void URModelBuilder::Load(USDFModel* InModelDescription, ARModel* OutModel)
{
  ModelDescription = InModelDescription;
  if(OutModel)
    {
      Model = OutModel;
      LoadLinks();
      LoadJoints();
      BuildKinematicTree();
    }
}

// Load links
void URModelBuilder::LoadLinks()
{
  for(USDFLink* Link : ModelDescription->Links)
    {
      URLink* TempLink = LinkFactory->Load(Model, Link);
      if(TempLink)
        {
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
  for(USDFJoint* Joint : ModelDescription->Joints)
    {
      URJoint* TempJoint = JointFactory->Load(Model, Joint);
      if(TempJoint)
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
  for(auto& Joint : Model->Joints)
    {
      URLink* Parent = *Model->Links.Find(Joint.Value->ParentName);
      URLink* Child = *Model->Links.Find(Joint.Value->ChildName);

      Joint.Value->SetParentChild(Parent, Child);
      SetConstraintPosition(Joint.Value);
      Joint.Value->Constraint->ConnectToComponents();

      Parent->AddJoint(Joint.Value);
    }
}

void URModelBuilder::SetConstraintPosition(URJoint* InJoint)
{
  if(InJoint->bUseParentModelFrame)
    {
      InJoint->Constraint->AttachToComponent(InJoint->Parent->GetCollision(), FAttachmentTransformRules::KeepRelativeTransform);
      InJoint->Constraint->SetWorldLocation(InJoint->Child->GetCollision()->GetComponentLocation());
      InJoint->Constraint->AddRelativeLocation(InJoint->Pose.GetLocation());
      InJoint->Constraint->AddRelativeRotation(InJoint->Pose.GetRotation());
    }
  else
    {
      //TODO Implement and check this case
      UE_LOG(LogTemp, Warning, TEXT("Does't use parent frame"));

      // InJoint->Constraint->SetPosition(InJoint);
      InJoint->Constraint->AttachToComponent(InJoint->Child->GetCollision(), FAttachmentTransformRules::KeepRelativeTransform);
    }
}
