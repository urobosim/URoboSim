#include "Factory/RROSCommunicationBuilder.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAServer.h"
#include "ROSCommunication/Publisher/RLidar2DPublisher.h"
#include "ROSCommunication/Publisher/RJointStatePublisher.h"
#include "ROSCommunication/Publisher/RJointTrajectoryControllerStatePublisher.h"
#include "ROSCommunication/Publisher/ROdomPublisher.h"
#include "ROSCommunication/Service/Client/RGetJointsClient.h"
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
  URROSCommunicationComponent *ROSCommunicationComponent = NewObject<URROSCommunicationComponent>(Owner, TEXT("ROSCommunicationComponent"));

  if (PublisherConfigurations.Num() > 0)
  {
    for (FRPublisherConfiguration PublisherConfiguration : PublisherConfigurations)
    {
      URPublisher *Publisher = CreatePublisher(Owner, PublisherConfiguration);
      if (Publisher)
      {
        UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *Publisher->GetName(), *Owner->GetName());
        Publisher->SetOwner();
        Publisher->SetPublishParameters(PublisherConfiguration.PublisherParameters);
        ROSCommunicationComponent->AddPublisher(Publisher);
      }
    }
  }

  if (SubscriberConfigurations.Num() > 0)
  {
    for (FRSubscriberConfiguration SubscriberConfiguration : SubscriberConfigurations)
    {
      URSubscriber *Subscriber = CreateSubscriber(Owner, SubscriberConfiguration);
      if (Subscriber)
      {
        UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *Subscriber->GetName(), *Owner->GetName());
        Subscriber->SetOwner();
        Subscriber->SetSubscriberParameters(SubscriberConfiguration.SubscriberParameters);
        ROSCommunicationComponent->AddSubscriber(Subscriber);
      }
    }
  }

  if (ServiceClientConfigurations.Num() > 0)
  {
    for (FRServiceClientConfiguration ServiceClientConfiguration : ServiceClientConfigurations)
    {
      URServiceClient *ServiceClient = CreateServiceClient(Owner, ServiceClientConfiguration);
      if (ServiceClient)
      {
        UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *ServiceClient->GetName(), *Owner->GetName());
        ServiceClient->SetOwner();
        ServiceClient->SetServiceClientParameters(ServiceClientConfiguration.ServiceClientParameters);
        ROSCommunicationComponent->AddServiceClient(ServiceClient);
      }
    }
  }

  if (ActionServerConfigurations.Num() > 0)
  {
    for (FRActionServerConfiguration ActionServerConfiguration : ActionServerConfigurations)
    {
      URActionServer *ActionServer = CreateActionServer(Owner, ActionServerConfiguration);
      if (ActionServer)
      {
        UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *ActionServer->GetName(), *Owner->GetName());
        ActionServer->SetActionServerParameters(ActionServerConfiguration.ActionServerParameters);
        ROSCommunicationComponent->AddActionServer(ActionServer);
      }
    }
  }

  ROSCommunicationComponent->RegisterComponent();
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
  else if (Cast<URLidar2DPublisherParameter>(PublisherConfiguration.PublisherParameters))
  {
    return NewObject<URLidar2DPublisher>(InOwner, TEXT("LidarPublisher"));
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

URServiceClient *URROSCommunicationBuilder::CreateServiceClient(ARModel *&InOwner, const FRServiceClientConfiguration &ServiceClientConfiguration)
{
  if (Cast<URGetJointsClientParameter>(ServiceClientConfiguration.ServiceClientParameters))
  {
    return NewObject<URGetJointsClient>(InOwner, TEXT("GetJointsClient"));
  }
  else if (Cast<URGetParamClientParameter>(ServiceClientConfiguration.ServiceClientParameters))
  {
    return NewObject<URGetParamClient>(InOwner, TEXT("GetParamClient"));
  }
  else
  {
    UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("ServiceClientParameters of %s not found, use default"), *ServiceClientConfiguration.ServiceClientParameters->GetName())
    return NewObject<URGetParamClient>(InOwner, TEXT("ServiceClient"));
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