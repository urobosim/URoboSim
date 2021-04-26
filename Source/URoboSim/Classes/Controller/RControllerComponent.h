// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "RPlugin.h"
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

	URController* GetController(FString ControllerName) const;

	void SetJointVelocities(TArray<FString> InJointNames, TArray<float> InJointVelocities);

public:
	UPROPERTY(EditAnywhere)
	FRControllerContainer Controllers;

protected:
  void BeginPlay() override;
};
