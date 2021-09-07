// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldControlGameInstance.h"
#include "AJointActor.h"
#include "ROSCommunication/KnowRobInterface.h"
#include "ROSCommunication/Service/Server/SetEnvironmentJointStatesServer.h"
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

  TMap<FString, AJointActor*> Joints;

  TSharedPtr<FROSBridgePublisher> Publisher;
  TSharedPtr<FSetEnvironmentJointStatesServerCallback> ServiceServer;

  UPROPERTY(EditAnywhere, Category= "Publish Environment Joint State")
  FString JointStatePublishTopic;

  UPROPERTY(EditAnywhere, Category= "Publish Environment Joint State")
  bool bEnableJointStatePublishing;

  UPROPERTY(EditAnywhere, Category= "Set Environment Joint State")
  bool bEnableSettingJointState;

  UPROPERTY(EditAnywhere, Category= "Set Environment Joint State")
  FEnableDrive EnableDrive;

  UPROPERTY(EditAnywhere, Category= "Set Environment Joint State")
    float ErrorTollerance = 0.01;

  UPROPERTY(EditAnywhere, Category= "KnowRob")
    UKnowrobInterface* KnowrobInterface;

  FString JointStatePublisherMessageType;
  FString SetJointStateServerType;
};
