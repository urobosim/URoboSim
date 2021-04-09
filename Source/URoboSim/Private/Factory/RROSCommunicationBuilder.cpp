#include "Factory/RROSCommunicationBuilder.h"
#include "ROSCommunication/Publisher/RJointStatePublisher.h"
#include "ROSCommunication/Publisher/RJointTrajectoryControllerStatePublisher.h"
#include "ROSCommunication/Publisher/ROdomPublisher.h"

DEFINE_LOG_CATEGORY_STATIC(LogRROSCommunicationBuilder, Log, All);

void URROSCommunicationBuilder::Init(const TArray<ARModel *> &InModels, const FRROSCommunicationConfiguration &InROSCommunicationConfiguration)
{
  if (InModels.Num() == 0)
  {
    UE_LOG(LogRROSCommunicationBuilder, Error, TEXT("No models found"))
    return;
  }

  Models = InModels;
  ROSCommunicationConfiguration = InROSCommunicationConfiguration;
}

void URROSCommunicationBuilder::Build(const TArray<FRPublisherConfiguration> &PublisherConfigurations,
                                      const TArray<FRSubscriberConfiguration> &SubscriberConfigurations,
                                      const TArray<FRServiceClientConfiguration> &ServiceClientConfigurations,
                                      const TArray<FRActionServerConfiguration> &ActionServerConfigurations)
{
  for (ARModel *&Model : Models)
  {
    URPluginComponent *ROSCommunicationComponent = NewObject<URROSCommunicationComponent>(Model, TEXT("ROSCommunicationComponent"));
    ROSCommunicationComponent->RegisterComponent();

    if (PublisherConfigurations.Num() > 0)
    {
      for (const FRPublisherConfiguration &PublisherConfiguration : PublisherConfigurations)
      {
        URPublisher *Publisher = CreatePublisher(Model, PublisherConfiguration);
        if (Publisher)
        {
          UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *Publisher->GetName(), *Model->GetName());
          Publisher->PublisherParameters = PublisherConfiguration.PublisherParameters;
          Cast<URROSCommunicationComponent>(ROSCommunicationComponent)->AddPublisher(Publisher);
        }
      }
    }
  }
}

URPublisher *URROSCommunicationBuilder::CreatePublisher(ARModel *&InOwner, const FRPublisherConfiguration &PublisherConfiguration)
{
  if (Cast<URJointStatePublisherParameter>(PublisherConfiguration.PublisherParameters))
  {
    return NewObject<URJointStatePublisher>(InOwner, TEXT("JointStatePublisher"));
  }
  else if (Cast<UROdomPublisherParameter>(PublisherConfiguration.PublisherParameters))
  {
    return NewObject<UROdomPublisher>(InOwner, TEXT("OdomPublisher"));
  }
  else
  {
    return NewObject<URPublisher>(InOwner, TEXT("Publisher"));
  }
}