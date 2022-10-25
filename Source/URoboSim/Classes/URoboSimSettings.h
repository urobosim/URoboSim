// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "URoboSimSettings.generated.h"

/**
 *
 */
UCLASS(Config=Ros, defaultconfig, meta = (DisplayName="URoboSim"))
class UROBOSIM_API UURoboSimSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

    UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
      bool bEnableURoboSim = true;

  UPROPERTY(EditAnywhere, Category= "Publish Environment Joint State")
  bool bEnableJointStatePublishing;

  UPROPERTY(EditAnywhere, Category= "Publish Environment Joint State")
  FString JointStatePublishTopic;

  UPROPERTY(EditAnywhere, Category= "Set Environment Joint State")
  bool bEnableSettingJointState;

  UPROPERTY(EditAnywhere, Category= "Set Environment Joint State")
  FEnableDrive EnableDrive;

  UPROPERTY(EditAnywhere, Category= "Set Environment Joint State")
    float ErrorTollerance = 0.01;
};
