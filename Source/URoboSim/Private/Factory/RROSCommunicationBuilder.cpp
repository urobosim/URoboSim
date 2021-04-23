#include "Factory/RROSCommunicationBuilder.h"
// #include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAServer.h"
#include "ROSCommunication/Publisher/JointStatePublisher.h"
// #include "ROSCommunication/Publisher/RJointTrajectoryControllerStatePublisher.h"
// #include "ROSCommunication/Publisher/ROdomPublisher.h"
// #include "ROSCommunication/Subscriber/RVelocityCommandSubscriber.h"
#include "ROSCommunication/Service/Client/RROSClient.h"

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
                                      const TArray<FRROSClientConfiguration> &ROSClientConfigurations,
                                      const TArray<FRROSServiceConfiguration> &ROSServiceConfigurations,
                                      const TArray<FRActionServerConfiguration> &ActionServerConfigurations)
{
  URROSCommunicationComponent *ROSCommunicationComponent = NewObject<URROSCommunicationComponent>(Owner, TEXT("ROSCommunicationComponent"));
  ROSCommunicationComponent->RegisterComponent();

  ROSCommunicationComponent->RosComunication.bUseGlobalHandler = ROSCommunicationConfiguration.bUseGlobalHandler;
  ROSCommunicationComponent->RosComunication.WebsocketIPAddr = ROSCommunicationConfiguration.WebsocketIPAddr;
  ROSCommunicationComponent->RosComunication.WebsocketPort = ROSCommunicationConfiguration.WebsocketPort;
  ROSCommunicationComponent->RosComunication.RobotName = Owner->GetName();

  if (PublisherConfigurations.Num() > 0)
  {
    for (FRPublisherConfiguration PublisherConfiguration : PublisherConfigurations)
    {
      URPublisher *Publisher = CreatePublisher(Owner, PublisherConfiguration);
      if (Publisher)
      {
        UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *Publisher->GetName(), *Owner->GetName());
        Publisher->SetPublishParameters(PublisherConfiguration.PublisherParameters);
        ROSCommunicationComponent->RosComunication.PublisherList.Add(Publisher->GetName(), Publisher);
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
        Subscriber->SetSubscriberParameters(SubscriberConfiguration.SubscriberParameters);
        ROSCommunicationComponent->RosComunication.SubscriberList.Add(Subscriber->GetName(), Subscriber);
      }
    }
  }

  if (ROSClientConfigurations.Num() > 0)
  {
    for (FRROSClientConfiguration ROSClientConfiguration : ROSClientConfigurations)
    {
      URROSClient *ROSClient = CreateROSClient(Owner, ROSClientConfiguration);
      if (ROSClient)
      {
        UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *ROSClient->GetName(), *Owner->GetName());
        ROSClient->SetROSClientParameters(ROSClientConfiguration.ROSClientParameters);
        ROSCommunicationComponent->RosComunication.ClientList.Add(ROSClient->GetName(), ROSClient);
      }
    }
  }

  // if (ActionServerConfigurations.Num() > 0)
  // {
  //   for (FRActionServerConfiguration ActionServerConfiguration : ActionServerConfigurations)
  //   {
  //     URActionServer *ActionServer = CreateActionServer(Owner, ActionServerConfiguration);
  //     if (ActionServer)
  //     {
  //       UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *ActionServer->GetName(), *Owner->GetName());
  //       ActionServer->SetActionServerParameters(ActionServerConfiguration.ActionServerParameters);
  //       ROSCommunicationComponent->AddActionServer(ActionServer);
  //     }
  //   }
  // }
}

URPublisher *URROSCommunicationBuilder::CreatePublisher(ARModel *&InOwner, const FRPublisherConfiguration &PublisherConfiguration)
{
  if (Cast<URJointStatePublisherParameter>(PublisherConfiguration.PublisherParameters))
  {
    return NewObject<URJointStatePublisher>(InOwner, TEXT("JointStatePublisher"));
  }
  // else if (Cast<UROdomPublisherParameter>(PublisherConfiguration.PublisherParameters))
  // {
  //   return NewObject<UROdomPublisher>(InOwner, TEXT("OdomPublisher"));
  // }
  // else if (Cast<URJointTrajectoryControllerStatePublisherParameter>(PublisherConfiguration.PublisherParameters))
  // {
  //   return NewObject<URJointTrajectoryControllerStatePublisher>(InOwner, TEXT("JointTrajectoryControllerStatePublisher"));
  // }
  else
  {
    UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("PublisherParameters of %s not found, use default"), *PublisherConfiguration.PublisherParameters->GetName())
    return NewObject<URPublisher>(InOwner, TEXT("Publisher"));
  }
}

URSubscriber *URROSCommunicationBuilder::CreateSubscriber(ARModel *&InOwner, const FRSubscriberConfiguration &SubscriberConfiguration)
{
  // if (Cast<URVelocityCommandSubscriberParameter>(SubscriberConfiguration.SubscriberParameters))
  // {
  //   return NewObject<URVelocityCommandSubscriber>(InOwner, TEXT("VelocityCommandSubscriber"));
  // }
  // else
  // {
  UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("SubscriberParameters of %s not found, use default"), *SubscriberConfiguration.SubscriberParameters->GetName())
  return NewObject<URSubscriber>(InOwner, TEXT("Subscriber"));
  // }
}

URROSClient *URROSCommunicationBuilder::CreateROSClient(ARModel *&InOwner, const FRROSClientConfiguration &ROSClientConfiguration)
{
  if (Cast<URJointControllerConfigurationClientParameter>(ROSClientConfiguration.ROSClientParameters))
  {
    return NewObject<URJointControllerConfigurationClient>(InOwner, TEXT("JointControllerConfigurationClient"));
  }
  // else if (Cast<URGetParamClientParameter>(ServiceClientConfiguration.ServiceClientParameters))
  // {
  //   return NewObject<URGetParamClient>(InOwner, TEXT("GetParamClient"));
  // }
  else
  {
    UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("ROSClientParameters of %s not found, use default"), *ROSClientConfiguration.ROSClientParameters->GetName())
    return NewObject<URROSClient>(InOwner, TEXT("ROSClient"));
  }
}

URROSService *URROSCommunicationBuilder::CreateROSService(ARModel *&InOwner, const FRROSServiceConfiguration &ROSServiceConfiguration)
{
  UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("ROSClientParameters of %s not found, use default"), *ROSServiceConfiguration.ROSServiceParameters->GetName())
  return NewObject<URROSService>(InOwner, TEXT("ROSService"));
}

URActionServer *URROSCommunicationBuilder::CreateActionServer(ARModel *&InOwner, const FRActionServerConfiguration &ActionServerConfiguration)
{
  // if (Cast<URFJTAServerParameter>(ActionServerConfiguration.ActionServerParameters))
  // {
  //   return NewObject<URFJTAServer>(InOwner, TEXT("FJTAServer"));
  // }
  // else
  // {
  UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("ActionServerParameters of %s not found, use default"), *ActionServerConfiguration.ActionServerParameters->GetName())
  return NewObject<URActionServer>(InOwner, TEXT("ActionServer"));
  // }
}