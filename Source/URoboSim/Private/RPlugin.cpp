// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#include "RPlugin.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPlugin, Log, All);

void URPluginComponent::OnComponentCreated()
{
	Super::OnComponentCreated();
	RegisterPlugin();
}

ARModel *URPluginComponent::GetOwner() const
{
	return Cast<ARModel>(Super::GetOwner());
}

void URPluginComponent::RegisterPlugin()
{
	if (GetOwner())
	{
		GetOwner()->AddPlugin(this);
		UE_LOG(LogRPlugin, Log, TEXT("%s is attached to %s"), *GetName(), *GetOwner()->GetName())
	}
}