// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSSubsystem.h"
#include "ROSCommunication/Service/Server/SetEnvironmentJointStatesServer.h"
#include "URoboSimSubsystem.generated.h"

class AJointActor;
class UKnowrobInterface;
/**
 *
 */
UCLASS()
class UROBOSIM_API UURoboSimSubsystem : public UROSGameInstanceSubsystem
{
	GENERATED_BODY()

 public:

  UURoboSimSubsystem();

  TMap<FString, AJointActor*> Joints;
  TSharedPtr<FROSBridgePublisher> Publisher;
  TSharedPtr<FSetEnvironmentJointStatesServerCallback> ServiceServer;

  UPROPERTY(EditAnywhere, Category= "KnowRob")
    UKnowrobInterface* KnowrobInterface;

  UPROPERTY()
  FString JointStatePublisherMessageType;

  UPROPERTY()
  FString SetJointStateServerType;


    // Begin USubsystem
  virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    // End USubsystem

};
