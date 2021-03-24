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
    PoseComponent->SetupAttachment(CollisionMeshes[0]);
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

// const FVector URLink::GetLinearVelocity() const
// {
//   if (CollisionMeshes.Num() > 0)
//   {
//     return CollisionMeshes[0]->GetPhysicsLinearVelocity();
//   }
//   else
//   {
//     return FVector(0.f);
//   }
// }

// const FVector URLink::GetAngularVelocity() const
// {
//   if (CollisionMeshes.Num() > 0)
//   {
//     return CollisionMeshes[0]->GetPhysicsAngularVelocity();
//   }
//   else
//   {
//     return FVector(0.f);
//   }
// }

// void URLink::SetLinearVelocity(const FVector &InLinearVelocity)
// {
// }

// void URLink::AddJoint(URJoint *InJoint)
// {
//   Joints.Add(InJoint);
// }

// URStaticMeshComponent *URLink::GetVisual()
// {
//   if (Visuals.Num() > 0)
//   {
//     return Visuals[0];
//   }
//   else
//   {
//     return nullptr;
//   }
// }

//   // for(auto& Visual : Visuals)
//   //   {
//   //     // Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
//   //     // Visual->SetCollisionProfileName(FName(TEXT("Spectator")));
//   //     Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
//   //   }
// }

// void URLink::EnableCollision()
// {
//   for (auto &Collision : Collisions)
//   {
//     // Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
//     // Collision->SetCollisionProfileName(FName(TEXT("PhysicsActor")));
//     Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
//     Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
//   }
// }

// URStaticMeshComponent *URLink::GetCollision()
// {
//   if (Collisions.Num() > 0)
//   {
//     return Collisions[0];
//   }
//   else
//   {
//     return nullptr;
//   }
// }

// URStaticMeshComponent *URLink::GetCollision(FString InCollisionName, bool bExactMatch)
// {
//   if (bExactMatch)
//   {
//     // if(GetName().Equals(InCollisionName))
//     //   {
//     //     return Collisions[0];
//     //   }

//     for (auto &Collision : Collisions)
//     {
//       if (Collision->GetName().Equals(InCollisionName))
//       {
//         return Collision;
//       }
//     }
//   }
//   else
//   {
//     // if(GetName().Contains(InCollisionName))
//     //   {
//     //     return Collisions[0];
//     //   }

//     for (auto &Collision : Collisions)
//     {
//       if (Collision->GetName().Contains(InCollisionName))
//       {
//         return Collision;
//       }
//     }
//   }
//   return nullptr;
// }

// TArray<class URJoint *> URLink::GetJoints()
// {
//   return Joints;
// }

// float URLink::GetNumCollisions()
// {
//   return Collisions.Num();
// }

// void URLink::UpdateVelocity(float InDeltaTime)
// {
//   for (URJoint *Joint : Joints)
//   {
//     Joint->UpdateVelocity(InDeltaTime);
//   }
// }
