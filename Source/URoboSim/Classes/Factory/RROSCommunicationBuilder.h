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
  virtual void Init(const TArray<ARModel *> &InModels, const FRROSCommunicationConfiguration &InROSCommunicationConfiguration);

  virtual void Build(const TArray<FRPublisherConfiguration> &PublisherConfigurations,
                     const TArray<FRSubscriberConfiguration> &SubscriberConfigurations,
                     const TArray<FRServiceClientConfiguration> &ServiceClientConfigurations,
                     const TArray<FRActionServerConfiguration> &ActionServerConfigurations);

  virtual URPublisher *CreatePublisher(ARModel *&InOwner, const FRPublisherConfiguration &PublisherConfiguration);

protected:
  UPROPERTY()
  TArray<ARModel *> Models;

  UPROPERTY()
  FRROSCommunicationConfiguration ROSCommunicationConfiguration;
};