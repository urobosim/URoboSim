#include "Factory/RROSCommunicationBuilder.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAServer.h"
#include "ROSCommunication/Publisher/RJointStatePublisher.h"
#include "ROSCommunication/Publisher/RJointTrajectoryControllerStatePublisher.h"
#include "ROSCommunication/Publisher/ROdomPublisher.h"
#include "ROSCommunication/Subscriber/RVelocityCommandSubscriber.h"

DEFINE_LOG_CATEGORY_STATIC(LogRROSCommunicationBuilder, Log, All);

void URROSCommunicationBuilder::Init(ARModel *&InOwner, const FRROSCommunicationConfiguration &InROSCommunicationConfiguration)
{
  if (!InOwner)
  {
    UE_LOG(LogRROSCommunicationBuilder, Error, TEXT("No models found"))
    return;
  }

  Owner = InOwner;
  ROSCommunicationConfiguration = InROSCommunicationConfiguration;
}

void URROSCommunicationBuilder::Build(const TArray<FRPublisherConfiguration> &PublisherConfigurations,
                                      const TArray<FRSubscriberConfiguration> &SubscriberConfigurations,
                                      const TArray<FRServiceClientConfiguration> &ServiceClientConfigurations,
                                      const TArray<FRActionServerConfiguration> &ActionServerConfigurations)
{
  URPluginComponent *ROSCommunicationComponent = NewObject<URROSCommunicationComponent>(Owner, TEXT("ROSCommunicationComponent"));
  ROSCommunicationComponent->RegisterComponent();

  if (PublisherConfigurations.Num() > 0)
  {
    for (const FRPublisherConfiguration &PublisherConfiguration : PublisherConfigurations)
    {
      URPublisher *Publisher = CreatePublisher(Owner, PublisherConfiguration);
      if (Publisher)
      {
        UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *Publisher->GetName(), *Owner->GetName());
        Publisher->PublisherParameters = PublisherConfiguration.PublisherParameters;
        Cast<URROSCommunicationComponent>(ROSCommunicationComponent)->AddPublisher(Publisher);
      }
    }
  }

  if (SubscriberConfigurations.Num() > 0)
  {
    for (const FRSubscriberConfiguration &SubscriberConfiguration : SubscriberConfigurations)
    {
      URSubscriber *Subscriber = CreateSubscriber(Owner, SubscriberConfiguration);
      if (Subscriber)
      {
        UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *Subscriber->GetName(), *Owner->GetName());
        Subscriber->SubscriberParameters = SubscriberConfiguration.SubscriberParameters;
        Cast<URROSCommunicationComponent>(ROSCommunicationComponent)->AddSubscriber(Subscriber);
      }
    }
  }

  if (ActionServerConfigurations.Num() > 0)
  {
    for (const FRActionServerConfiguration &ActionServerConfiguration : ActionServerConfigurations)
    {
      URActionServer *ActionServer = CreateActionServer(Owner, ActionServerConfiguration);
      if (ActionServer)
      {
        UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *ActionServer->GetName(), *Owner->GetName());
        ActionServer->ActionServerParameters = ActionServerConfiguration.ActionServerParameters;
        Cast<URROSCommunicationComponent>(ROSCommunicationComponent)->AddActionServer(ActionServer);
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
  else if (Cast<URJointTrajectoryControllerStatePublisherParameter>(PublisherConfiguration.PublisherParameters))
  {
    return NewObject<URJointTrajectoryControllerStatePublisher>(InOwner, TEXT("JointTrajectoryControllerStatePublisher"));
  }
  else
  {
    UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("PublisherParameters of %s not found, use default"), *PublisherConfiguration.PublisherParameters->GetName())
    return NewObject<URPublisher>(InOwner, TEXT("Publisher"));
  }
}

URSubscriber *URROSCommunicationBuilder::CreateSubscriber(ARModel *&InOwner, const FRSubscriberConfiguration &SubscriberConfiguration)
{
  if (Cast<URVelocityCommandSubscriberParameter>(SubscriberConfiguration.SubscriberParameters))
  {
    return NewObject<URVelocityCommandSubscriber>(InOwner, TEXT("VelocityCommandSubscriber"));
  }
  else
  {
    UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("SubscriberParameters of %s not found, use default"), *SubscriberConfiguration.SubscriberParameters->GetName())
    return NewObject<URSubscriber>(InOwner, TEXT("Subscriber"));
  }
}

URActionServer *URROSCommunicationBuilder::CreateActionServer(ARModel *&InOwner, const FRActionServerConfiguration &ActionServerConfiguration)
{
  if (Cast<URFJTAServerParameter>(ActionServerConfiguration.ActionServerParameters))
  {
    return NewObject<URFJTAServer>(InOwner, TEXT("FJTAServer"));
  }
  else
  {
    UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("ActionServerParameters of %s not found, use default"), *ActionServerConfiguration.ActionServerParameters->GetName())
    return NewObject<URActionServer>(InOwner, TEXT("ActionServer"));
  }
}