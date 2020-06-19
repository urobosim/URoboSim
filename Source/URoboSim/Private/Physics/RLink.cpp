// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Physics/RLink.h"
#include "Physics/RJoint.h"
#include "Physics/RModel.h"


URLink::URLink()
{
}

void URLink::SetPose(FTransform InPose)
{
	Pose = InPose;
}

void URLink::SetPose(FVector InLocation, FQuat InRotation)
{
	Pose.SetLocation(InLocation);
	Pose.SetRotation(InRotation);
}

void URLink::AddJoint(URJoint* InJoint)
{
	Joints.Add(InJoint);
}

URStaticMeshComponent* URLink::GetVisual()
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

URStaticMeshComponent* URLink::GetCollision()
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

TArray<class URJoint*> URLink::GetJoints()
{
  return Joints;
}

float URLink::GetNumCollisions()
{
	return Collisions.Num();
}

void URLink::UpdateVelocity(float InDeltaTime)
{
  for(URJoint* Joint : Joints)
    {
      Joint->UpdateVelocity(InDeltaTime);
    }
}

void URLink::SetNextVelocities()
{
	if(Collisions.Num()>0)
	{
		Collisions[0]->SetNextVelocities();
	}
}
