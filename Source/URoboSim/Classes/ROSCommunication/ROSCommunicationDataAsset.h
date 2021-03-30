
// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ROSCommunication/RROSService.h"
#include "ROSCommunication/RROSClient.h"
#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "ROSCommunication/Actions/RActionServer.h"
#include "ROSCommunicationDataAsset.generated.h"
/**
 *
 */

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew , Category = "ROSCommunication")
class UROBOSIM_API UROSCommunicationConfiguration : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Basic Information")
          bool bUseGlobalHandler = false;

	UPROPERTY(EditAnywhere, Category = "Basic Information")
          FString WebsocketIPAddr = TEXT("127.0.0.1");

	UPROPERTY(EditAnywhere, Category = "Basic Information")
          uint32 WebsocketPort = 9090;

	UPROPERTY(EditAnywhere, Category = "Basic Information")
          FString RobotName = TEXT("None");
};

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew , Category = "Controller")
class UROBOSIM_API URBaseCommunicationConfiguration : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
          FString Topic = TEXT("None");

	UPROPERTY(EditAnywhere)
          FString Name = TEXT("None");
};

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew )
class UROBOSIM_API URSubscriberConfiguration : public URBaseCommunicationConfiguration
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
          TSubclassOf<URSubscriber> Type;
};

// Configuratation for spezific Controller
UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew )
class UROBOSIM_API URPublisherConfiguration : public URBaseCommunicationConfiguration
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
          TSubclassOf<URPublisher> Type;
};

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew )
class UROBOSIM_API URServiceClientConfiguration : public URBaseCommunicationConfiguration
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
          TSubclassOf<URROSClient> Type;
};

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew )
class UROBOSIM_API URServiceServerConfiguration : public URBaseCommunicationConfiguration
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
          TSubclassOf<URROSService> Type;
};

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew )
class UROBOSIM_API URActionConfiguration : public URBaseCommunicationConfiguration
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
          FString ControllerName = TEXT("None");

	UPROPERTY(EditAnywhere)
          TSubclassOf<URActionServer> Type;
};

UCLASS(BlueprintType, Category = "ROSCommunication")
class UROBOSIM_API UROSCommunicationDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	//// Constructor, set default values

	// Names of the robots for which this configration asset is used
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Basic Information")
	TArray<FString> RobotNames;

	// Basic ROS infos such as ip, port ...
	UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear, Category = "Basic Information")
	UROSCommunicationConfiguration* ROSCommunicationConfigurations;

	// Subscriber Configuration
	UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear, Category = "Communication Information")
	TArray<URSubscriberConfiguration*> SubscriberConfiguration;

	// Publisher Configuration
	UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear, Category = "Communication Information")
	TArray<URPublisherConfiguration*> PublisherConfiguration;

	// Service client Configuration
	UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear, Category = "Communication Information")
	TArray<URServiceClientConfiguration*> ServiceClientConfiguration;

	// Service server Configuration
	UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear, Category = "Communication Information")
	TArray<URServiceServerConfiguration*> ServiceServerConfiguration;

	// Action Configuration
	UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear, Category = "Communication Information")
	TArray<URActionConfiguration*> ActionConfiguration;
};
