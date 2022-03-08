#pragma once

#include "ROSCommunication/RROSCommunicationComponent.h"
#include "ROSCommunication/RROSCommunicationDataAsset.h"
// clang-format off
#include "RROSCommunicationBuilder.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URROSCommunicationBuilder : public UObject
{
  GENERATED_BODY()

public:
  void Init(ARModel *&InOwner, const FRROSCommunicationConfiguration &InROSCommunicationConfiguration);

  void Build(const TArray<FRPublisherConfiguration> &PublisherConfigurations,
             const TArray<FRSubscriberConfiguration> &SubscriberConfigurations,
             const TArray<FRServiceServerConfiguration> &ServiceServerConfigurations,
             const TArray<FRServiceClientConfiguration> &ServiceClientConfigurations,
             const TArray<FRActionServerConfiguration> &ActionServerConfigurations);

protected:
  URPublisher *CreatePublisher(ARModel *&InOwner, const FRPublisherConfiguration &PublisherConfiguration);

  URSubscriber *CreateSubscriber(ARModel *&InOwner, const FRSubscriberConfiguration &SubscriberConfiguration);

  URServiceServer *CreateServiceServer(ARModel *&InOwner, const FRServiceServerConfiguration &ServiceServerConfiguration);

  URServiceClient *CreateServiceClient(ARModel *&InOwner, const FRServiceClientConfiguration &ServiceClientConfiguration);

  URActionServer *CreateActionServer(ARModel *&InOwner, const FRActionServerConfiguration &ActionServerConfiguration);

protected:
  ARModel *Owner;

  FRROSCommunicationConfiguration ROSCommunicationConfiguration;
};