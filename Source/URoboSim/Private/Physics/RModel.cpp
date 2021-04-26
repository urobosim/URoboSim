// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Physics/RModel.h"

// Sets default values
ARModel::ARModel()
{
  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.TickGroup = TG_PrePhysics;
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

void ARModel::AddLink(URLink *Link)
{
  Links.Add(Link->GetName(), Link);
}

void ARModel::AddJoint(URJoint *Joint)
{
  Joints.Add(Joint->GetName(), Joint);
}

TArray<FJointState> ARModel::GetJointStates() const
{
  TArray<FJointState> JointStates;

  for (const TPair<FString, URJoint *> &Joint : Joints)
  {
    JointStates.Add(FJointState(Joint.Key, Joint.Value->GetJointPosition(), Joint.Value->GetJointVelocity()));
  }

  return JointStates;
}
