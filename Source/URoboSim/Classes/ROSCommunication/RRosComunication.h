// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Components/ActorComponent.h"
#include "Physics/RJoint.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "ROSCommunication/Service/Client/RROSClient.h"
#include "ROSCommunication/Service/Server/RROSService.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
#include "Physics/RModel.h"
#include "ROSUtilities.h"
#include "RRosComunication.generated.h"

class URControllerComponent;

USTRUCT(Blueprintable)
struct FRROSComunicationContainer
{
	GENERATED_BODY()

 public:

    TSharedPtr<FROSBridgeHandler> Handler;

	// UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
	// URRosComunicationInterface* Interface;

	UPROPERTY()
	URControllerComponent* ControllerComponent;

    UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
	bool bUseGlobalHandler;

    UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
        FString WebsocketIPAddr;

    UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
        uint32 WebsocketPort;

    UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
        FString RobotName;

	UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
	TMap<FString, URPublisher*> PublisherList;

	UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
	TMap<FString, URSubscriber*> SubscriberList;

	UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
	TMap<FString, URROSClient*> ClientList;

	UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
	TMap<FString, URROSService*> ServiceProviderList;

	UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
	TMap<FString, URActionServer*> ActionServerList;

	FRROSComunicationContainer();

	virtual ~FRROSComunicationContainer(){};
	virtual void InitHandler();

	virtual void InitAllPublisher() ;
	virtual void InitAllSubscriber() ;
	virtual void InitAllServiceProvider() ;
	virtual void InitAllClients() ;
	virtual void InitAllActionServer() ;

	virtual void Init();
	virtual void DeInit();
	virtual void Tick();

 protected:

};
