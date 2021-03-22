// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Physics/RModel.h"
#include "Controller/RControllerComponent.h"

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
  Init();
}

void ARModel::Init()
{
  if (Links.Num() > 0)
  {
    Links[0]->SetEnableGravity(EnableGravity.bBase);
  }
  else
  {
    UE_LOG(LogRModel, Error, TEXT("Model %s has no links"), *GetName())
  }
  for (URJoint *&Joint : Joints)
  {
    Joint->GetChild()->SetEnableGravity(EnableGravity.bLinks);
    Joint->Init();
  }
  for (URLink *&Link : Links)
  {
    Link->SetSimulatePhysics(bSimulatePhysics);
    Link->Init();
  }
  for (UActorComponent *&Plugin : Plugins)
  {
    URControllerComponent *Controllers = Cast<URControllerComponent>(Plugin);
    if (Controllers)
    {
      Controllers->Init();
    }
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
