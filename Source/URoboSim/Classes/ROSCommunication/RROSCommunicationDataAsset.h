// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "Engine/DataAsset.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Service/Client/RServiceClient.h"
#include "ROSCommunication/Service/Server/RServiceServer.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
// clang-format off
#include "RROSCommunicationDataAsset.generated.h"
// clang-format on
/**
 *
 */

USTRUCT()
struct FRROSCommunicationConfiguration
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, Category = "Basic Information")
  FString WebsocketIPAddr = TEXT("127.0.0.1");

  UPROPERTY(EditAnywhere, Category = "Basic Information")
  uint32 WebsocketPort = 9090;
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

USTRUCT()
struct FRSubscriberConfiguration
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  URSubscriberParameter *SubscriberParameters;
};

USTRUCT()
struct FRServiceServerConfiguration
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  URServiceServerParameter *ServiceServerParameters;
};

USTRUCT()
struct FRServiceClientConfiguration
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  URServiceClientParameter *ServiceClientParameters;
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
  UPROPERTY(EditAnywhere, Category = "Basic Information")
  FString RobotName;

  // Basic ROS infos such as ip, port ...
  UPROPERTY(EditAnywhere, NoClear, Category = "Basic Information")
  FRROSCommunicationConfiguration ROSCommunicationConfiguration;

  // Publisher Configuration
  UPROPERTY(EditAnywhere, Category = "Communication Information")
  TArray<FRPublisherConfiguration> PublisherConfiguration;

  // Subscriber Configuration
  UPROPERTY(EditAnywhere, Category = "Communication Information")
  TArray<FRSubscriberConfiguration> SubscriberConfiguration;

  // Service server Configuration
  UPROPERTY(EditAnywhere, Category = "Communication Information")
  TArray<FRServiceServerConfiguration> ServiceServerConfiguration;

  // Service client Configuration
  UPROPERTY(EditAnywhere, Category = "Communication Information")
  TArray<FRServiceClientConfiguration> ServiceClientConfiguration;

  // Action Configuration
  UPROPERTY(EditAnywhere, Category = "Communication Information")
  TArray<FRActionServerConfiguration> ActionServerConfiguration;
};