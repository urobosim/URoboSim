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

void URPluginComponent::RegisterPlugin()
{
	ARModel *Owner = Cast<ARModel>(GetOwner());
	if (Owner)
	{
		Owner->AddPlugin(this);
		UE_LOG(LogRPluginComponent, Log, TEXT("%s is attached to %s"), *GetName(), *GetOwner()->GetName())
	}
}