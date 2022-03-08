// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "RPluginComponent.h"
#include "RController.h"
// clang-format off
#include "RControllerComponent.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URControllerComponent : public URPluginComponent
{
	GENERATED_BODY()

public:
	URControllerComponent();

public:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void AddController(URController *&Controller) { Controllers.Add(Controller); }

	URController* GetController(const FString &ControllerName) const;

protected:
	void Init() override;

public:
	UPROPERTY(EditAnywhere, Instanced)
	TArray<URController *> Controllers;
};
