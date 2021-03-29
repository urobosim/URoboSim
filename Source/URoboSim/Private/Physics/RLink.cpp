// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Physics/RLink.h"

DEFINE_LOG_CATEGORY_STATIC(LogRLink, Log, All);

URLink::URLink()
{
  PoseComponent = CreateDefaultSubobject<USceneComponent>(*(GetName() + TEXT("_pose")));
}

void URLink::Init()
{
}

void URLink::SetPose(const FTransform &Pose)
{
  if (CollisionMeshes.Num() > 0)
  {
    PoseComponent->AttachToComponent(CollisionMeshes[0], FAttachmentTransformRules::KeepWorldTransform);
    PoseComponent->SetWorldTransform(Pose);
  }
  else
  {
    UE_LOG(LogRLink, Error, TEXT("Link %s doesn't have any collision meshes, SetPose failed"), *GetName())
  }
}

void URLink::SetEnableGravity(const bool &bGravityEnabled)
{
  for (UStaticMeshComponent *CollisionMesh : CollisionMeshes)
  {
    CollisionMesh->SetEnableGravity(bGravityEnabled);
  }
}

void URLink::SetSimulatePhysics(const bool &bSimulate)
{
  for (UStaticMeshComponent *CollisionMesh : CollisionMeshes)
  {
    CollisionMesh->SetSimulatePhysics(bSimulate);
  }
}

void URLink::AttachToComponent(USceneComponent *Parent)
{
  if (CollisionMeshes.Num() > 0)
  {
    CollisionMeshes[0]->AttachToComponent(Parent, FAttachmentTransformRules::KeepWorldTransform);
  }
  else
  {
    UE_LOG(LogRLink, Error, TEXT("Link %s doesn't have any collision meshes, AttachToComponent failed"), *GetName())
  }
}

void URLink::DisableCollision()
{
  if (CollisionMeshes.Num() > 0)
  {
    UE_LOG(LogRLink, Log, TEXT("Disable collision for %s"), *CollisionMeshes[0]->GetName())
    CollisionMeshes[0]->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
  }
  else
  {
    UE_LOG(LogRLink, Error, TEXT("Link %s doesn't have any collision meshes, DisableCollision failed"), *GetName())
  }
}