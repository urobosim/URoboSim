// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Physics/RModel.h"
// clang-format off
#include "RPluginComponent.generated.h"
// clang-format on

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URPluginComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void OnComponentCreated() override;

public:
	virtual void BeginPlay() override;

	virtual void Init() {}

	virtual ARModel *GetOwner() const;

protected:
	virtual void RegisterPlugin();
};