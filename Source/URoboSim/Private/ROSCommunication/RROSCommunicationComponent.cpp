#include "ROSCommunication/RROSCommunicationComponent.h"
#include "Controller/RControllerComponent.h"
#include "ROSBridgeGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "ROSCommunication/Service/Client/RServiceClient.h"
#include "ROSCommunication/Action/Server/RActionServer.h"

DEFINE_LOG_CATEGORY_STATIC(LogRROSCommunicationComponent, Log, All);

FRROSCommunicationContainer::FRROSCommunicationContainer()
    : FRROSCommunicationContainer::FRROSCommunicationContainer(TEXT("127.0.0.1"), 9090)
{
}

FRROSCommunicationContainer::FRROSCommunicationContainer(const FString &InWebsocketIPAddr, const uint32 &InWebsocketPort)
    : WebsocketIPAddr(InWebsocketIPAddr), WebsocketPort(InWebsocketPort)
{
}

void FRROSCommunicationContainer::Init(UObject* InOwner)
{
  Owner = InOwner;

  if(Owner)
    {
      if(bUseGlobalIP)
        {
          UROSBridgeGameInstance* GI = Cast<UROSBridgeGameInstance>(UGameplayStatics::GetGameInstance(Owner));
          if(GI)
            {
              WebsocketIPAddr = GI->ROSBridgeServerHost;
              WebsocketPort = GI->ROSBridgeServerPort;
            }
          else
            {
              UE_LOG(LogRROSCommunicationComponent, Log, TEXT("Wrong GameInstance"));
            }

        }
      InitPublishers();
      InitSubscribers();
      InitServiceClients();
      InitActionServers();
    }
}

void FRROSCommunicationContainer::InitPublishers()
{
  for (URPublisher *&Publisher : Publishers)
  {
    Publisher->Connect(WebsocketIPAddr, WebsocketPort);
  }
}

void FRROSCommunicationContainer::InitSubscribers()
{
  for (URSubscriber *&Subscriber : Subscribers)
  {
    Subscriber->Connect(WebsocketIPAddr, WebsocketPort);
  }
}

void FRROSCommunicationContainer::InitServiceClients()
{
  for (URServiceClient *&ServiceClient : ServiceClients)
  {
    ServiceClient->Connect(WebsocketIPAddr, WebsocketPort);
  }
}

void FRROSCommunicationContainer::InitActionServers()
{
  for (URActionServer *&ActionServer : ActionServers)
  {
    ActionServer->Connect(WebsocketIPAddr, WebsocketPort);
  }
}

void FRROSCommunicationContainer::Tick()
{
  for (URPublisher *&Publisher : Publishers)
  {
    Publisher->Tick();
  }
  for (URSubscriber *&Subscriber : Subscribers)
  {
    Subscriber->Tick();
  }
  for (URServiceClient *&ServiceClient : ServiceClients)
  {
    ServiceClient->Tick();
  }
  for (URActionServer *&ActionServer : ActionServers)
  {
    ActionServer->Tick();
  }
}

void FRROSCommunicationContainer::DeInit()
{
  UE_LOG(LogRROSCommunicationComponent, Log, TEXT("Deinitilize ROSCommunication"))
  for (URPublisher *&Publisher : Publishers)
  {
    Publisher->Disconnect();
  }
  for (URSubscriber *&Subscriber : Subscribers)
  {
    Subscriber->Disconnect();
  }
  for (URServiceClient *&ServiceClient : ServiceClients)
  {
    ServiceClient->Disconnect();
  }
  for (URActionServer *&ActionServer : ActionServers)
  {
    ActionServer->Disconnect();
  }
  if (Handler.IsValid())
  {
    Handler->Disconnect();
  }
}

URROSCommunicationComponent::URROSCommunicationComponent(const FString &InWebsocketIPAddr, const uint32 &InWebsocketPort) : URROSCommunicationComponent::URROSCommunicationComponent()
{
  ROSCommunication = FRROSCommunicationContainer(InWebsocketIPAddr, InWebsocketPort);
}

URROSCommunicationComponent::URROSCommunicationComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
  PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void URROSCommunicationComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
  ROSCommunication.Tick();
}

void URROSCommunicationComponent::Init()
{
  ROSCommunication.Init(this);
}

void URROSCommunicationComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  ROSCommunication.DeInit();
  Super::EndPlay(EndPlayReason);
}
