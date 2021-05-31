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

TArray<URJoint *> ARModel::GetJoints() const
{
  TArray<URJoint *> JointArray;
  for (const TPair<FString, URJoint *> &Joint : Joints)
  {
    JointArray.Add(Joint.Value);
  }
  return JointArray;
}

URJoint *ARModel::GetJoint(const FString &JointName) const
{
  if (Joints.Contains(JointName))
  {
    return Joints[JointName];
  }
  else
  {
    UE_LOG(LogRModel, Error, TEXT("%s not found in %s"), *JointName, *GetName())
    return nullptr;
  }
}

TArray<URLink *> ARModel::GetLinks() const
{
  TArray<URLink *> LinkArray;
  for (const TPair<FString, URLink *> &Link : Links)
  {
    LinkArray.Add(Link.Value);
  }
  return LinkArray;
}

URLink *ARModel::GetLink(const FString &LinkName) const
{
  if (Links.Contains(LinkName))
  {
    return Links[LinkName];
  }
  else
  {
    UE_LOG(LogRModel, Error, TEXT("%s not found in %s"), *LinkName, *GetName())
    return nullptr;
  }
}

bool ARModel::AddPlugin(URPluginComponent *InPlugin)
{
  if (URPluginComponent *Plugin = GetPlugin(InPlugin->GetName()))
  {
    UE_LOG(LogRModel, Warning, TEXT("Plugin %s was found in %s, replace..."), *InPlugin->GetName(), *GetName())
    Plugin = InPlugin;
    return false;
  }
  else
  {
    Plugins.Add(InPlugin->GetName(), InPlugin);
    return true;
  }
}

URPluginComponent *ARModel::GetPlugin(const FString &PluginName) const
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
  for (TPair<FString, URPluginComponent *> Plugin : Plugins)
  {
    if (URControllerComponent *ControllerComponent = Cast<URControllerComponent>(Plugin.Value))
    {
      return ControllerComponent->GetController(ControllerName);
    }
  }
  UE_LOG(LogRModel, Error, TEXT("ControllerComponent not found in %s"), *GetName())
  return nullptr;
}