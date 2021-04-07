#include "Factory/RROSCommunicationBuilder.h"
#include "ROSCommunication/Publisher/RJointStatePublisher.h"

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
    URPluginComponent *ROSCommunicationComponent = Model->GetPlugin(TEXT("ROSCommunicationComponent"));
    if (ROSCommunicationComponent)
    {
      UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("ROSCommunicationComponent of %s found"), *Model->GetName())
    }
    else
    {
      UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("ROSCommunicationComponent of %s not found, create one"), *Model->GetName())
      ROSCommunicationComponent = NewObject<URROSCommunicationComponent>(Model, TEXT("ROSCommunicationComponent"));
      if (!Cast<URROSCommunicationComponent>(ROSCommunicationComponent))
      {
        UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("ControllerComponent of %s could not be created"), *Model->GetName())
        return;
      }
      else
      {
        ROSCommunicationComponent->RegisterComponent();
      }
    }

    if (PublisherConfigurations.Num() > 0)
    {
      URPublisherBuilder *PublisherBuilder = NewObject<URPublisherBuilder>();
      for (const FRPublisherConfiguration &PublisherConfiguration : PublisherConfigurations)
      {
        // UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *PublisherConfiguration .GetName(), *Model->GetName());
        if (Cast<URJointStatePublisherParameter>(PublisherConfiguration.PublisherParameters))
        {
          PublisherBuilder = NewObject<URJointStatePublisherBuilder>();
        }
        URPublisher *Publisher = PublisherBuilder->CreatePublisher(Model, PublisherConfiguration);
        Cast<URROSCommunicationComponent>(ROSCommunicationComponent)->AddPublisher(Publisher);
      }
    }
  }
}

URPublisher *URPublisherBuilder::CreatePublisher(ARModel *&InOwner, const FRPublisherConfiguration &PublisherConfiguration)
{
  Publisher = NewObject<URPublisher>(InOwner, TEXT("Publisher"));
  Configure(PublisherConfiguration);
  return Publisher;
}

URPublisher *URJointStatePublisherBuilder::CreatePublisher(ARModel *&InOwner, const FRPublisherConfiguration &PublisherConfiguration)
{
  Publisher = NewObject<URJointStatePublisher>(InOwner, TEXT("JointStatePublisher"));
  Configure(PublisherConfiguration);
  return Publisher;
}

void URJointStatePublisherBuilder::Configure(const FRPublisherConfiguration &PublisherConfiguration)
{
  if (Cast<URJointStatePublisherParameter>(PublisherConfiguration.PublisherParameters))
  {
    if (!PublisherConfiguration.PublisherParameters->Topic.Equals(TEXT("")))
    {
      Publisher->Topic = PublisherConfiguration.PublisherParameters->Topic;
    }
    Cast<URJointStatePublisher>(Publisher)->FrameId = Cast<URJointStatePublisherParameter>(PublisherConfiguration.PublisherParameters)->FrameId;    
  }
}