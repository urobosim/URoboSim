// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "RPlugin.h"
#include "RController.h"
#include "RControllerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API URControllerComponent : public URPluginComponent
{
    GENERATED_BODY()
public:
	URControllerComponent();
	~URControllerComponent();

	virtual URController* ControllerList(FString ControllerName);

	virtual void SetJointVelocities(TArray<FString> InJointNames, TArray<float> InJointVelocities);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	FRControllerContainer Controller;

protected:
    virtual void BeginPlay() override;
    virtual FString GetPluginName() override;
};
