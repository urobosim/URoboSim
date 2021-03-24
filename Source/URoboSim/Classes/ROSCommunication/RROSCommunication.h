
// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Components/ActorComponent.h"
#include "Physics/RJoint.h"
#include "Physics/RModel.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
// #include "ROSCommunication/Actions/RActionServer.h"
#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Service/Client/RServiceClient.h"
// #include "ROSCommunication/RROSService.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
// clang-format off
#include "RROSCommunication.generated.h"
// clang-format on

class URControllerComponent;

USTRUCT(Blueprintable)
struct FRROSCommunicationContainer
{
  GENERATED_BODY()

public:
  FRROSCommunicationContainer();
  virtual ~FRROSCommunicationContainer(){};

public:
  virtual void Init();
  virtual void DeInit();
  virtual void Tick();

public:
  UPROPERTY()
  URControllerComponent *ControllerComponent;

protected:
  virtual void InitPublishers();
  virtual void InitSubscribers();
  // virtual void InitServiceServers();
  virtual void InitServiceClients();
  // virtual void InitActionServers();

protected:
  TSharedPtr<FROSBridgeHandler> Handler;

  UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
  FString WebsocketIPAddr;

  UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
  uint32 WebsocketPort;

  UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
  TArray<URPublisher *> Publishers;

  UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
  TArray<URSubscriber *> Subscribers;

  UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
  TArray<URServiceClient *> ServerClients;

  // UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
  // TArray<URROSService *> ServiceProviderList;

  // UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
  // TArray<URActionServer *> ActionServerList;
};