// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Physics/RModel.h"
#include "Controller/RControllerComponent.h"
#include "Sensor/RSensorComponent.h"

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
  UE_LOG(LogRModel, Log, TEXT("Model %s is spawned"), *GetName())
  Super::BeginPlay();
}

void ARModel::Init()
{
  UE_LOG(LogRModel, Log, TEXT("Model %s initializes with %i plugins"), *GetName(), Plugins.Num())
  if (Links.Num() > 0)
  {
    Links[0]->SetEnableGravity(EnableGravity.bBase);
  }
  else
  {
    UE_LOG(LogRModel, Warning, TEXT("Model %s has no links"), *GetName())
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
}

URJoint *ARModel::GetJoint(const FString &JointName) const
{
  URJoint *const *JointPtr = Joints.FindByPredicate([&](URJoint *Joint) { return Joint->GetName().Equals(JointName); });
  if (JointPtr)
  {
    return *JointPtr;
  }
  else
  {
    UE_LOG(LogRModel, Error, TEXT("Joint %s not found in %s"), *JointName, *GetName())
    return nullptr;
  }
}

URLink *ARModel::GetBaseLink() const
{
  if (Links.Num() > 0)
  {
    return Links[0];
  }
  else
  {
    UE_LOG(LogRModel, Error, TEXT("Model %s doesn't have any links"), *GetName())
    return nullptr;
  }
}

bool ARModel::AddPlugin(URPluginComponent *InPlugin)
{
  URPluginComponent *Plugin = GetPlugin(InPlugin->GetName());
  if (Plugin)
  {
    UE_LOG(LogRModel, Warning, TEXT("Plugin %s was found in %s, replace..."), *InPlugin->GetName(), *GetName())
    Plugin = InPlugin;
    return false;
  }
  else
  {
    Plugins.Add(InPlugin);
    return true;
  }
}

URPluginComponent *ARModel::GetPlugin(const FString &PluginName) const
{
  URPluginComponent *const *PluginPtr = Plugins.FindByPredicate([&](URPluginComponent *Plugin) { return Plugin->GetName().Equals(PluginName); });
  if (PluginPtr)
  {
    return *PluginPtr;
  }
  else
  {
    return nullptr;
  }
}

URController *ARModel::GetController(const FString &ControllerName) const
{
  URControllerComponent *ControllerComponent = FindComponentByClass<URControllerComponent>();
  if (ControllerComponent)
  {
    return ControllerComponent->GetController(ControllerName);
  }
  else
  {
    UE_LOG(LogRModel, Error, TEXT("%s not found in %s"), *ControllerName, *GetName())
    return nullptr;
  }
}

URSensor *ARModel::GetSensor(const FString &SensorName) const
{
  URSensorComponent *SensorComponent = FindComponentByClass<URSensorComponent>();
  if (SensorComponent)
  {
    return SensorComponent->GetSensor(SensorName);
  }
  else
  {
    UE_LOG(LogRModel, Error, TEXT("%s not found in %s"), *SensorName, *GetName())
    return nullptr;
  }
}