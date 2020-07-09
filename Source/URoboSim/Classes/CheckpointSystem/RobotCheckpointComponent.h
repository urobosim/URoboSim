// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UCheckpointComponent.h"
#include "RobotCheckpointComponent.generated.h"

/**
 *
 */
UCLASS(ClassGroup=(Checkpoint), meta=(BlueprintSpawnableComponent) )
class UROBOSIM_API URobotCheckpointComponent : public UCheckpointComponent
{
	GENERATED_BODY()

public:
        URobotCheckpointComponent();

 protected:

	// Called when the game starts
	virtual void BeginPlay() override;


};
