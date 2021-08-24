// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Physics/RLink.h"
#include "Physics/RJoint.h"
#include "Physics/RModel.h"


URLink::URLink()
{
}

void URLink::AddJoint(URJoint* InJoint)
{
	Joints.Add(InJoint);
}

UStaticMeshComponent* URLink::GetVisual()
{
	if(Visuals.Num()>0)
	{
		return Visuals[0];
	}
	else
	{
		return nullptr;
	}
}

void URLink::DisableCollision()
{
  for(auto& Collision : Collisions)
    {

      UE_LOG(LogTemp, Error, TEXT("disable collision for %s"), *Collision->GetName());
      Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
      // Collision->SetCollisionProfileName(FName(TEXT("Spectator")));
      // Collision->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    }

  // for(auto& Visual : Visuals)
  //   {
  //     // Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
  //     // Visual->SetCollisionProfileName(FName(TEXT("Spectator")));
  //     Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
  //   }
}

void URLink::EnableCollision()
{
  for(auto& Collision : Collisions)
    {
      // Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
      // Collision->SetCollisionProfileName(FName(TEXT("PhysicsActor")));
      Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
      Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
    }
}

UStaticMeshComponent* URLink::GetCollision()
{
        if(Collisions.Num()>0)
	{
		return Collisions[0];
	}
	else
	{
		return nullptr;
	}
}

UStaticMeshComponent* URLink::GetCollision(FString InCollisionName, bool bExactMatch)
{
      if(bExactMatch)
        {
          // if(GetName().Equals(InCollisionName))
          //   {
          //     return Collisions[0];
          //   }

          for(auto& Collision : Collisions)
            {
              if(Collision->GetName().Equals(InCollisionName))
                {
                  return Collision;
                }
            }
        }
      else
        {
          // if(GetName().Contains(InCollisionName))
          //   {
          //     return Collisions[0];
          //   }

          for(auto& Collision : Collisions)
            {
              if(Collision->GetName().Contains(InCollisionName))
                {
                  return Collision;
                }
            }
        }
  return nullptr;
}

TArray<class URJoint*> URLink::GetJoints()
{
  return Joints;
}

float URLink::GetNumCollisions()
{
	return Collisions.Num();
}
