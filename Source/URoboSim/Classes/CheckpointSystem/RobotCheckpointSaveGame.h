// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorCheckpointSaveGame.h"
#include "RobotCheckpointSaveGame.generated.h"

/**
 *
 */
UCLASS()
class UROBOSIM_API URobotCheckpointSaveGame : public UActorCheckpointSaveGame
{
	GENERATED_BODY()

protected:

    UPROPERTY(VisibleAnywhere)
    TMap<FString, float> JointStates;

public:


    virtual void SetJointState(TMap<FString, float> InJointState);
    virtual TMap<FString, float> GetJointStates();

};
