#include "Factory/RModelBuilder.h"

DEFINE_LOG_CATEGORY_STATIC(LogRModelBuilder, Log, All);

// Sets default values
URModelBuilder::URModelBuilder()
{
  LinkBuilder = CreateDefaultSubobject<URLinkBuilder>(TEXT("LinkBuilder"));
}

// Load model
void URModelBuilder::LoadSDF(USDFModel *&SDFModel)
{
  if (Model)
  {
    SwapBaseLinkToFirstIndex(SDFModel);
    LoadLinks(SDFModel);
    LoadJoints(SDFModel);
  }
}

void URModelBuilder::LoadSDF(USDFModel *&SDFModel, const FTransform &WorldPose)
{
  LinkBuilder->SetWorldPose(WorldPose);
  LoadSDF(SDFModel);
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
  
  for (USDFJoint *&SDFJoint : SDFModel->Joints)
  {
    if (SDFJoint->Type.Equals("continuous"))
    {
      JointBuilder = NewObject<URContinuousJointBuilder>(this, TEXT("ContinousJointBuilder"));
    }
    else if (SDFJoint->Type.Equals("revolute"))
    {
      JointBuilder = NewObject<URRevoluteJointBuilder>(this, TEXT("RevoluteJointBuilder"));
    }
    else if (SDFJoint->Type.Equals("prismatic"))
    {
      JointBuilder = NewObject<URPrismaticJointBuilder>(this, TEXT("PrismaticJointBuilder"));
    }
    else if (SDFJoint->Type.Equals("screw"))
    {
      JointBuilder = NewObject<URPrismaticJointBuilder>(this, TEXT("PrismaticJointBuilder"));
    }
    else
    {
      UE_LOG(LogRModelBuilder, Error, TEXT("Type %s of Joint %s not implemented"), *SDFJoint->Type, *SDFJoint->GetName())
      return;
    }
    JointBuilder->Model = Model;
    if (!JointBuilder->LoadSDF(SDFJoint))
    {
      UE_LOG(LogRModelBuilder, Error, TEXT("Creation of Joint %s failed"), *SDFJoint->GetName());
    }
  }
}