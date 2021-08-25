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
 public:
    URoboSimGameInstance();
  virtual void OnStart() override;
  virtual void Tick(float DeltaTime) override;

  TArray<AJointActor*> Joints;

  TSharedPtr<FROSBridgePublisher> Publisher;

  UPROPERTY(EditAnywhere)
  FString Topic;

  UPROPERTY(EditAnywhere)
  bool bEnableJointStatePublishing;

  FString MessageType ;
};
