// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "RPluginComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPluginComponent, Log, All);

void URPluginComponent::OnComponentCreated()
{
  Super::OnComponentCreated();
  RegisterPlugin();
}

void URPluginComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogRPluginComponent, Log, TEXT("Initialize %s of %s"), *GetName(), *GetOwner()->GetName())
	Init();
}

ARModel* URPluginComponent::GetOwner() const
{
  return Cast<ARModel>(Super::GetOwner());
}

void URPluginComponent::RegisterPlugin()
{
  ARModel *Owner = GetOwner();
  if (Owner)
  {
    Owner->AddPlugin(this);
    UE_LOG(LogRPluginComponent, Log, TEXT("%s is attached to %s"), *GetName(), *GetOwner()->GetName())
  }
  else
  {
    UE_LOG(LogRPluginComponent, Error, TEXT("%s not Attached to a ARModel"), *GetName())
  }
}
