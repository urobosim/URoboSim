// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "Engine/DataAsset.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Publisher/RPublisherParameter.h"
#include "ROSCommunication/Service/Client/RServiceClient.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
// clang-format off
#include "RROSCommunicationDataAsset.generated.h"
// clang-format on

USTRUCT()
struct FRROSCommunicationConfiguration
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, Category = "Basic Information")
  FString WebsocketIPAddr = TEXT("127.0.0.1");

  UPROPERTY(EditAnywhere, Category = "Basic Information")
  uint32 WebsocketPort = 9393;
};

// Configuratation for spezific Controller
USTRUCT()
struct FRPublisherConfiguration
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  URPublisherParameter *PublisherParameters;
};

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URSubscriberParameter : public UObject
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  FString Topic;
};

USTRUCT()
struct FRSubscriberConfiguration
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  URSubscriberParameter *SubscriberParameters;
};

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URServiceClientParameter : public UObject
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  FString Topic;
};

USTRUCT()
struct FRServiceClientConfiguration
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  URServiceClientParameter *ServiceClientParameters;
};

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URActionServerParameter : public UObject
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  FString Topic;
};

USTRUCT()
struct FRActionServerConfiguration
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  URActionServerParameter *ActionServerParameters;
};

UCLASS(BlueprintType, Category = "ROSCommunication")
class UROBOSIM_API URROSCommunicationDataAsset : public UDataAsset
{
  GENERATED_BODY()

public:
  // Names of the robots for which this configration asset is used
  UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Basic Information")
  TArray<FString> RobotNames;

  // Basic ROS infos such as ip, port ...
  UPROPERTY(EditAnywhere, export, noclear, Category = "Basic Information")
  FRROSCommunicationConfiguration ROSCommunicationConfiguration;

  // Publisher Configuration
  UPROPERTY(EditAnywhere, Category = "Communication Information")
  TArray<FRPublisherConfiguration> PublisherConfiguration;

  // Subscriber Configuration
  UPROPERTY(EditAnywhere, Category = "Communication Information")
  TArray<FRSubscriberConfiguration> SubscriberConfiguration;

  // Service client Configuration
  UPROPERTY(EditAnywhere, Category = "Communication Information")
  TArray<FRServiceClientConfiguration> ServiceClientConfiguration;

  // Action Configuration
  UPROPERTY(EditAnywhere, Category = "Communication Information")
  TArray<FRActionServerConfiguration> ActionServerConfiguration;
};