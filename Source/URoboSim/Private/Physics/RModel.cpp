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

UActorComponent *ARModel::GetPlugin(const FString &PluginName) const
{
  if (Plugins.Contains(PluginName))
  {
    return Plugins[PluginName];
  }
  else
  {
    UE_LOG(LogRModel, Error, TEXT("%s not found in %s"), *PluginName, *GetName())
    return nullptr;
  }
}

URController *ARModel::GetController(const FString &ControllerName) const
{
  URControllerComponent *ControllerComponent = Cast<URControllerComponent>(GetPlugin(TEXT("Controller")));
  if (ControllerComponent)
  {
    return ControllerComponent->GetController(ControllerName);
  }
  else
  {
    return nullptr;
  }
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
