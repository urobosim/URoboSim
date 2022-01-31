#include "Factory/RROSCommunicationBuilder.h"
#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAServer.h"
#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GCAServer.h"
#include "ROSCommunication/Action/Server/PR2HeadAction/PR2HAServer.h"
#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAServer.h"
#include "ROSCommunication/Publisher/2DLidarPublisher.h"
#include "ROSCommunication/Publisher/JointStatePublisher.h"
#include "ROSCommunication/Publisher/JointTrajectoryControllerStatePublisher.h"
#include "ROSCommunication/Publisher/RDonbotOdomPublisher.h"
#include "ROSCommunication/Publisher/RTFPublisher.h"
#include "ROSCommunication/Service/Client/RJointControllerConfigurationClient.h"
#include "ROSCommunication/Service/Client/RJointStateConfigurationClient.h"
#include "ROSCommunication/Service/Server/CheckVisibilityServer.h"
#include "ROSCommunication/Service/Server/RRegistrationServer.h"
#include "ROSCommunication/Service/Server/RSimulationCommandsServer.h"
#include "ROSCommunication/Subscriber/JointStateReplaySubscriber.h"
#include "ROSCommunication/Subscriber/JointStateSubscriber.h"
#include "ROSCommunication/Subscriber/OdometrieReplaySubscriber.h"
#include "ROSCommunication/Subscriber/VelocityCommandSubscriber.h"
#include "ROSCommunication/Subscriber/GripperCommandSubscriber.h"
#include "ROSCommunication/Subscriber/TFSubscriber.h"

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
                                      const TArray<FRServiceServerConfiguration> &ServiceServerConfigurations,
                                      const TArray<FRServiceClientConfiguration> &ServiceClientConfigurations,
                                      const TArray<FRActionServerConfiguration> &ActionServerConfigurations)
{
  URROSCommunicationComponent *ROSCommunicationComponent = NewObject<URROSCommunicationComponent>(Owner, TEXT("ROSCommunicationComponent"));
  ROSCommunicationComponent->ROSCommunication.WebsocketIPAddr = ROSCommunicationConfiguration.WebsocketIPAddr;
  ROSCommunicationComponent->ROSCommunication.WebsocketPort = ROSCommunicationConfiguration.WebsocketPort;

  if (PublisherConfigurations.Num() > 0)
  {
    for (FRPublisherConfiguration PublisherConfiguration : PublisherConfigurations)
    {
      URPublisher *Publisher = CreatePublisher(Owner, PublisherConfiguration);
      if (Publisher)
      {
        UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *Publisher->GetName(), *Owner->GetName());
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
        Subscriber->SetSubscriberParameters(SubscriberConfiguration.SubscriberParameters);
        ROSCommunicationComponent->AddSubscriber(Subscriber);
      }
    }
  }

  if (ServiceServerConfigurations.Num() > 0)
  {
    for (FRServiceServerConfiguration ServiceServerConfiguration : ServiceServerConfigurations)
    {
      URServiceServer *ServiceServer = CreateServiceServer(Owner, ServiceServerConfiguration);
      if (ServiceServer)
      {
        UE_LOG(LogRROSCommunicationBuilder, Log, TEXT("Create %s of %s"), *ServiceServer->GetName(), *Owner->GetName());
        ServiceServer->SetServiceServerParameters(ServiceServerConfiguration.ServiceServerParameters);
        ROSCommunicationComponent->AddServiceServer(ServiceServer);
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
    return NewObject<URJointStatePublisher>(InOwner);
  }
  else if (Cast<UR2DLidarPublisherParameter>(PublisherConfiguration.PublisherParameters))
  {
    return NewObject<UR2DLidarPublisher>(InOwner);
  }
  else if (Cast<URDonbotOdomPublisherParameter>(PublisherConfiguration.PublisherParameters))
  {
    return NewObject<URDonbotOdomPublisher>(InOwner);
  }
  else if (Cast<UROdomPublisherParameter>(PublisherConfiguration.PublisherParameters))
  {
    return NewObject<UROdomPublisher>(InOwner);
  }
  else if (Cast<URTFPublisherParameter>(PublisherConfiguration.PublisherParameters))
  {
    return NewObject<URTFPublisher>(InOwner);
  }
  else if (Cast<URJointTrajectoryControllerStatePublisherParameter>(PublisherConfiguration.PublisherParameters))
  {
    return NewObject<URJointTrajectoryControllerStatePublisher>(InOwner);
  }
  else
  {
    UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("PublisherParameters of %s not found, use default"), *PublisherConfiguration.PublisherParameters->GetName())
    return NewObject<URPublisher>(InOwner);
  }
}

URSubscriber *URROSCommunicationBuilder::CreateSubscriber(ARModel *&InOwner, const FRSubscriberConfiguration &SubscriberConfiguration)
{
  if (Cast<URJointStateReplaySubscriberParameter>(SubscriberConfiguration.SubscriberParameters))
  {
    return NewObject<URJointStateReplaySubscriber>(InOwner);
  }
  else if (Cast<URJointStateSubscriberParameter>(SubscriberConfiguration.SubscriberParameters))
  {
    return NewObject<URJointStateSubscriber>(InOwner);
  }
  else if (Cast<URVelocityCommandSubscriberParameter>(SubscriberConfiguration.SubscriberParameters))
  {
    return NewObject<URVelocityCommandSubscriber>(InOwner);
  }
  else if (Cast<URGripperCommandSubscriberParameter>(SubscriberConfiguration.SubscriberParameters))
  {
    return NewObject<URGripperCommandSubscriber>(InOwner);
  }
  else if (Cast<URTFSubscriberParameter>(SubscriberConfiguration.SubscriberParameters))
  {
    return NewObject<URTFSubscriber>(InOwner);
  }
  else
  {
    UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("SubscriberParameters of %s not found, use default"), *SubscriberConfiguration.SubscriberParameters->GetName())
    return NewObject<URSubscriber>(InOwner);
  }
}

URServiceServer *URROSCommunicationBuilder::CreateServiceServer(ARModel *&InOwner, const FRServiceServerConfiguration &ServiceServerConfiguration)
{
  if (Cast<URRegistrationServerParameter>(ServiceServerConfiguration.ServiceServerParameters))
  {
    return NewObject<URRegistrationServer>(InOwner);
  }
  else if (Cast<URCheckVisibilityServerParameter>(ServiceServerConfiguration.ServiceServerParameters))
  {
    return NewObject<URCheckVisibilityServer>(InOwner);
  }
  else if (Cast<URSimulationCommandsServerParameter>(ServiceServerConfiguration.ServiceServerParameters))
  {
    return NewObject<URSimulationCommandsServer>(InOwner);
  }
  else
  {
    UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("ROSClientParameters of %s not found, use default"), *ServiceServerConfiguration.ServiceServerParameters->GetName())
    return NewObject<URServiceServer>(InOwner);
  }
}

URServiceClient *URROSCommunicationBuilder::CreateServiceClient(ARModel *&InOwner, const FRServiceClientConfiguration &ServiceClientConfiguration)
{
  if (Cast<URJointControllerConfigurationClientParameter>(ServiceClientConfiguration.ServiceClientParameters))
  {
    return NewObject<URJointControllerConfigurationClient>(InOwner);
  }
  else if (Cast<URJointStateConfigurationClientParameter>(ServiceClientConfiguration.ServiceClientParameters))
  {
    return NewObject<URJointStateConfigurationClient>(InOwner);
  }
  else
  {
    UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("ServiceClientParameters of %s not found, use default"), *ServiceClientConfiguration.ServiceClientParameters->GetName())
    return NewObject<URServiceClient>(InOwner);
  }
}

URActionServer *URROSCommunicationBuilder::CreateActionServer(ARModel *&InOwner, const FRActionServerConfiguration &ActionServerConfiguration)
{
  if (Cast<URFJTAServerParameter>(ActionServerConfiguration.ActionServerParameters))
  {
    return NewObject<URFJTAServer>(InOwner);
  }
  else if (Cast<URPOAServerParameter>(ActionServerConfiguration.ActionServerParameters))
  {
    return NewObject<URPOAServer>(InOwner);
  }
  else if (Cast<URPR2GCAServerParameter>(ActionServerConfiguration.ActionServerParameters))
  {
    return NewObject<URPR2GCAServer>(InOwner);
  }
  else if (Cast<URPR2HAServerParameter>(ActionServerConfiguration.ActionServerParameters))
  {
    return NewObject<URPR2HAServer>(InOwner);
  }
  else
  {
    UE_LOG(LogRROSCommunicationBuilder, Warning, TEXT("ActionServerParameters of %s not found, use default"), *ActionServerConfiguration.ActionServerParameters->GetName())
    return NewObject<URActionServer>(InOwner);
  }
}
