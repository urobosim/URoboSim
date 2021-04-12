
// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "ROSCommunication/Service/Client/RServiceClient.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
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

public:
  void Init();

  void DeInit();
  
  void Tick();

public:
  void AddPublisher(URPublisher *&InPublisher) { Publishers.Add(InPublisher); }

  void AddSubscriber(URSubscriber *&InSubscriber) { Subscribers.Add(InSubscriber); }

  void AddServiceClient(URServiceClient *&InServiceClient) { ServiceClients.Add(InServiceClient); }

  void AddActionServer(URActionServer *&InActionServer) { ActionServers.Add(InActionServer); }

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

  UPROPERTY(EditAnywhere, Instanced)
  TArray<URPublisher *> Publishers;

  UPROPERTY(EditAnywhere, Instanced)
  TArray<URSubscriber *> Subscribers;

  UPROPERTY(EditAnywhere, Instanced)
  TArray<URServiceClient *> ServiceClients;

  UPROPERTY(EditAnywhere, Instanced)
  TArray<URActionServer *> ActionServers;
};