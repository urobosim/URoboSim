#include "Factory/RModelBuilder.h"

DEFINE_LOG_CATEGORY_STATIC(LogRModelBuilder, Log, All);

// Sets default values
URModelBuilder::URModelBuilder()
{
  LinkBuilder = CreateDefaultSubobject<URLinkBuilder>(TEXT("LinkBuilder"));
  JointBuilder = CreateDefaultSubobject<URJointBuilder>(TEXT("JointBuilder"));
}

// Load model
void URModelBuilder::LoadSDF(USDFModel *&SDFModel)
{
  if (Model)
  {
    SwapBaseLinkToFirstIndex(SDFModel);
    LoadLinks(SDFModel);
    LoadJoints(SDFModel);
    LockBaseLink();
  }
}

void URModelBuilder::SwapBaseLinkToFirstIndex(USDFModel *&SDFModel)
{
  // Find BaseLink
  USDFLink *BaseLink = *SDFModel->Links.FindByPredicate([&](USDFLink *Link) {
    for (USDFJoint *&Joint : SDFModel->Joints)
    {
      if (Link->GetName().Equals(Joint->Child))
      {
        return false;
      }
    }
    return true;
  });

  // Swap BaseLink
  for (int32 i = 0; i < SDFModel->Links.Num(); i++)
  {
    if (SDFModel->Links[i] == BaseLink)
    {
      UE_LOG(LogRModelBuilder, Log, TEXT("Found BaseLink %s"), *BaseLink->GetName())
      SDFModel->Links.Swap(0, i);
      break;
    }
  }
}

// Load links
void URModelBuilder::LoadLinks(USDFModel *&SDFModel)
{
  LinkBuilder->Model = Model;
  for (USDFLink *&SDFLink : SDFModel->Links)
  {
    if (!LinkBuilder->LoadSDF(SDFLink))
    {
      UE_LOG(LogRModelBuilder, Error, TEXT("Creation of Link %s failed"), *SDFLink->GetName());
    }
  }
}

// Load joints
void URModelBuilder::LoadJoints(USDFModel *&SDFModel)
{
  JointBuilder->Model = Model;
  for (USDFJoint *&SDFJoint : SDFModel->Joints)
  {
    if (!JointBuilder->LoadSDF(SDFJoint))
    {
      UE_LOG(LogRModelBuilder, Error, TEXT("Creation of Joint %s failed"), *SDFJoint->GetName());
    }
  }
}

// Lock base link
void URModelBuilder::LockBaseLink()
{
  Model->GetLinks()[0]->GetCollisionMeshes()[0]->SetConstraintMode(EDOFMode::XYPlane);
}