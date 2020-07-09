// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorCheckpointHandler.h"
#include "CheckpointSystem/RobotCheckpointSaveGame.h"
#include "RobotCheckpointHandler.generated.h"

/**
 *
 */
UCLASS()
class UROBOSIM_API URobotCheckpointHandler : public UActorCheckpointHandler
{
	GENERATED_BODY()

public:

    virtual USaveGame* CreateSaveGameFile() override;
    virtual void GetContent() override;
    virtual void ResetCheckpoint() override;

protected:
    virtual void SetActorState(UActorCheckpointSaveGame* InActorState) override;
    virtual void SetJointState(URobotCheckpointSaveGame* InRobotState);
};
