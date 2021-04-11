
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

  FRROSCommunicationContainer(const FString &InWebsocketIPAddr, const uint32 &InWebsocketPort);

  ~FRROSCommunicationContainer(){};

public:
  void Init();

  void DeInit();
  
  void Tick();

public:
  void AddPublisher(URPublisher *&InPublisher) { Publishers.Add(InPublisher); }

  void AddSubscriber(URSubscriber *&InSubscriber) { Subscribers.Add(InSubscriber); }

  void AddActionServer(URActionServer *&InActionServer) { ActionServers.Add(InActionServer); }

public:
  URControllerComponent *ControllerComponent;

protected:
  void InitPublishers();
  void InitSubscribers();
  void InitServiceClients();
  void InitActionServers();

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