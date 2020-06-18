// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeGameInstance.h"
// #include "ROSCommunication/ResetLevelServer.h"
#include "WorldControlGameInstance.generated.h"

/**
 *
 */
UCLASS()
class UROBOSIM_API UWorldControlGameInstance : public UROSBridgeGameInstance
{
  GENERATED_BODY()

    virtual void OnStart() override;

  // virtual void Tick(float DeltaTime) override;



};
