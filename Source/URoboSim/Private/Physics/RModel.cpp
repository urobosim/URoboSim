// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Physics/RModel.h"

DEFINE_LOG_CATEGORY_STATIC(LogRModel, Log, All);

// Sets default values
ARModel::ARModel()
{
  PrimaryActorTick.bCanEverTick = true;
  PrimaryActorTick.TickGroup = TG_PrePhysics;
  bSimulatePhysics = true;
}

// Called every frame
void ARModel::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
  for (URJoint *&Joint : Joints)
  {
    Joint->Tick(DeltaTime);
  }
}

// Called when the game starts or when spawned
void ARModel::BeginPlay()
{
  Super::BeginPlay();

  Links[0]->SetEnableGravity(EnableGravity.bBase);
  for (URJoint *&Joint : Joints)
  {
    Joint->GetChild()->SetEnableGravity(EnableGravity.bLinks);
    Joint->BeginPlay();
  }
  for (URLink *&Link : Links)
  {
    Link->SetSimulatePhysics(bSimulatePhysics);
    Link->BeginPlay();
  }
}

// const TArray<FJointState> ARModel::GetJointState() const
// {
//   TArray<FJointState> JointState;
//   for (const URJoint *Joint : Joints)
//   {
//     JointState.Add(FJointState(Joint->GetName(), Joint->GetPosition(), Joint->GetVelocity()));
//   }

//   return JointState;
// }
