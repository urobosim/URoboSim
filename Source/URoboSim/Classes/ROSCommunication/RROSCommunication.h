
// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "ROSCommunication/Service/Client/RServiceClient.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
#include "Controller/RControllerComponent.h"
// #include "ROSCommunication/RROSService.h"
// clang-format off
#include "RROSCommunication.generated.h"
// clang-format on

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
  virtual void InitServiceClients();
  virtual void InitActionServers();

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
  TArray<URServiceClient *> ServiceClients;

  UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
  TArray<URActionServer *> ActionServers;
};