// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldControlGameInstance.h"
#include "AJointActor.h"
#include "URoboSimGameInstance.generated.h"

/**
 *
 */
UCLASS()
class UROBOSIM_API URoboSimGameInstance : public UWorldControlGameInstance
{
  GENERATED_BODY()

    virtual void OnStart() override;

 public:

  virtual void Tick(float DeltaTime) override;

  TArray<AJointActor*> Joints;

  TSharedPtr<FROSBridgePublisher> Publisher;

  FString Topic;
  FString MessageType ;
};
