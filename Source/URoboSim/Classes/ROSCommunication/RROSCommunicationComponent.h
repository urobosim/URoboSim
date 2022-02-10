// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "RPluginComponent.h"
#include "RROSCommunication.h"
// clang-format off
#include "RROSCommunicationComponent.generated.h"
// clang-format on

class URPublisher;
class URSubscriber;
class URServiceServer;
class URServiceClient;
class URActionServer;

USTRUCT(Blueprintable)
struct FRROSCommunicationContainer
{
	GENERATED_BODY()

public:
	FRROSCommunicationContainer();

	FRROSCommunicationContainer(const FString &InWebsocketIPAddr, const uint32 &InWebsocketPort);

public:
	void Init(UObject* InOwner);

	void DeInit();

	void Tick();

public:
	void AddPublisher(URPublisher *&InPublisher) { Publishers.Add(InPublisher); }

	void AddSubscriber(URSubscriber *&InSubscriber) { Subscribers.Add(InSubscriber); }

	void AddServiceServer(URServiceServer *&InServiceServer) { ServiceServers.Add(InServiceServer); }

	void AddServiceClient(URServiceClient *&InServiceClient) { ServiceClients.Add(InServiceClient); }

	void AddActionServer(URActionServer *&InActionServer) { ActionServers.Add(InActionServer); }

public:
	UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
	FString WebsocketIPAddr;

	UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
	uint32 WebsocketPort;

protected:
	void InitPublishers();

	void InitSubscribers();

	void InitServiceClients();

	void InitActionServers();

protected:
	UPROPERTY()
        UObject* Owner;

	UPROPERTY(EditAnywhere)
        bool bUseGlobalIP = true;

	TSharedPtr<FROSBridgeHandler> Handler;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<URPublisher *> Publishers;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<URSubscriber *> Subscribers;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<URServiceServer *> ServiceServers;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<URServiceClient *> ServiceClients;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<URActionServer *> ActionServers;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URROSCommunicationComponent final : public URPluginComponent
{
	GENERATED_BODY()

public:
	URROSCommunicationComponent(const FString &InWebsocketIPAddr, const uint32 &InWebsocketPort);

	URROSCommunicationComponent();

protected:
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

public:
	void AddPublisher(URPublisher *&InPublisher) { ROSCommunication.AddPublisher(InPublisher); }

	void AddSubscriber(URSubscriber *&InSubscriber) { ROSCommunication.AddSubscriber(InSubscriber); }

	void AddServiceServer(URServiceServer *&InServiceServer) { ROSCommunication.AddServiceServer(InServiceServer); }

	void AddServiceClient(URServiceClient *&InServiceClient) { ROSCommunication.AddServiceClient(InServiceClient); }

	void AddActionServer(URActionServer *&InActionServer) { ROSCommunication.AddActionServer(InActionServer); }

protected:
	void Init() override;

public:
	UPROPERTY(EditAnywhere)
	FRROSCommunicationContainer ROSCommunication;
};
