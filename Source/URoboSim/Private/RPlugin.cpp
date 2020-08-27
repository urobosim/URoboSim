// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "RPlugin.h"

void URPluginComponent::OnComponentCreated()
{
  Super::OnComponentCreated();
  RegisterPlugin();
}

void URPluginComponent::RegisterPlugin()
{
  Model = Cast<ARModel>(GetOwner());
  FString PluginName = GetPluginName();
  if(Model)
    {
      Model->Plugins.Add(PluginName, this);
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
