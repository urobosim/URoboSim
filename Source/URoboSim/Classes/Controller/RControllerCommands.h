// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Components/ActorComponent.h"
#include "RControllerCommands.generated.h"

class ARModel;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API URobotCommandsComponent : public UActorComponent
{
    GENERATED_BODY()
public:

	void Init();
	virtual void MoveWithVelocity(FVector InVelocity, float DeltaTime);
protected:

	virtual void BeginPlay() override;

	UPROPERTY()
	ARModel* Model;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API UPR2CommandsComponent : public URobotCommandsComponent
{
    GENERATED_BODY()
public:

};
