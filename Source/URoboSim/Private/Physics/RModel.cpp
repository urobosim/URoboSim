// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Physics/RModel.h"
#include "Physics/RJoint.h"


// Sets default values
ARModel::ARModel()
{
  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.TickGroup = TG_PrePhysics;
}

// Dtor
ARModel::~ARModel()
{
}

// Called when the game starts or when spawned
void ARModel::BeginPlay()
{
  Super::BeginPlay();
}

// Called every frame
void ARModel::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void ARModel::AddLink(URLink* Link)
{
  Links.Add(Link->GetName(), Link);
}

void ARModel::AddJoint(URJoint* Joint)
{
  Joints.Add(Joint->GetName(), Joint);
}

FJointState ARModel::GetJointState()
{
  FJointState JointState;

  for(auto& Joint : Joints)
    {
      JointState.JointNames.Add(Joint.Key);
      JointState.JointPositions.Add(Joint.Value->GetJointPosition());
      JointState.JointVelocities.Add(Joint.Value->GetJointVelocity());
    }

  return JointState;
}
