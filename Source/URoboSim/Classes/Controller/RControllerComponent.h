// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "RPluginComponent.h"
#include "RController.h"
// clang-format off
#include "RControllerComponent.generated.h"
// clang-format on

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API URControllerComponent : public URPluginComponent
{
	GENERATED_BODY()

public:
	URControllerComponent();

public:
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Init();

	void AddController(URController *Controller) { Controllers.Add(Controller); }

	URController* GetController(FString ControllerName) const;

public:
	UPROPERTY(EditAnywhere, Instanced)
	TArray<URController *> Controllers;

protected:
  void BeginPlay() override;
};
