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
  virtual void Init(ARModel *&InOwner, const FRROSCommunicationConfiguration &InROSCommunicationConfiguration);

  virtual void Build(const TArray<FRPublisherConfiguration> &PublisherConfigurations,
                     const TArray<FRSubscriberConfiguration> &SubscriberConfigurations,
                     const TArray<FRServiceClientConfiguration> &ServiceClientConfigurations,
                     const TArray<FRActionServerConfiguration> &ActionServerConfigurations);

protected:
  virtual URPublisher *CreatePublisher(ARModel *&InOwner, const FRPublisherConfiguration &PublisherConfiguration);

  virtual URSubscriber *CreateSubscriber(ARModel *&InOwner, const FRSubscriberConfiguration &SubscriberConfiguration);

  virtual URActionServer *CreateActionServer(ARModel *&InOwner, const FRActionServerConfiguration &ActionServerConfiguration);

protected:
  ARModel * Owner;

  FRROSCommunicationConfiguration ROSCommunicationConfiguration;
};