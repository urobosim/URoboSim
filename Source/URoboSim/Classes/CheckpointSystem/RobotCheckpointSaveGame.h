// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RUtilityClasses.h"
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
    TMap<FString, FJointState> JointStates;

public:


    virtual void SetJointState(TMap<FString, FJointState> InJointState);
    virtual TMap<FString, FJointState> GetJointStates();

};
