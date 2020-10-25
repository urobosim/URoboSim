// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "RPlugin.h"

void URPluginComponent::OnComponentCreated()
{
  Super::OnComponentCreated();
  RegisterPlugin();
}

ARModel* URPluginComponent::GetOwner()
{
  return Cast<ARModel>(Super::GetOwner());
}

void URPluginComponent::RegisterPlugin()
{
  if (GetOwner())
  {
    GetOwner()->Plugins.Add(GetPluginName(), this);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("%s not Attached to a ARModel"), *GetName());
  }
}

FString URPluginComponent::GetPluginName()
{
  return TEXT("DefaultPluginName");
}
