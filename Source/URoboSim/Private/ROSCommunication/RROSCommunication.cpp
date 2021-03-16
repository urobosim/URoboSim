#include "ROSCommunication/RROSCommunication.h"
#include "Controller/RController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRROSCommunicationContainer, Log, All);

FRROSCommunicationContainer::FRROSCommunicationContainer()
{
  WebsocketIPAddr = TEXT("127.0.0.1");
  WebsocketPort = 9090;
  ControllerComponent = nullptr;
}

void FRROSCommunicationContainer::Init()
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  if (Handler.IsValid())
  {
    Handler->Connect();
    InitPublishers();
    // InitAllSubscriber();
    // InitAllServiceProvider();
    InitServiceClients();
    // InitAllActionServer();
  }
  else
  {
    UE_LOG(LogRROSCommunicationContainer, Error, TEXT("No FROSBridgeHandler created."))
  }
}

void FRROSCommunicationContainer::InitPublishers()
{
  for (URPublisher *&Publisher : Publishers)
  {
    Publisher->Init(ControllerComponent->GetOwner(), WebsocketIPAddr, WebsocketPort);
  }
}

// void FRROSCommunicationContainer::InitAllSubscriber()
// {
//   for (auto &Subscriber : SubscriberList)
//   {
//     Subscriber.Value->Init(ControllerComponent->GetOwner(), Handler);
//   }
// }

// void FRROSCommunicationContainer::InitAllServiceProvider()
// {
//   for (auto &ServiceProvider : ServiceProviderList)
//   {
//     ServiceProvider.Value->Init(ControllerComponent, Handler, ServiceProvider.Key);
//   }
// }

void FRROSCommunicationContainer::InitServiceClients()
{
  for (URServiceClient *&ServerClient : ServerClients)
  {
    ServerClient->Init(ControllerComponent, Handler);
  }
}

// void FRROSCommunicationContainer::InitAllActionServer()
// {
//   for (auto &ActionServer : ActionServerList)
//   {
//     ActionServer.Value->Init(WebsocketIPAddr, WebsocketPort, ControllerComponent->GetOwner());
//   }
// }

void FRROSCommunicationContainer::Tick()
{
  if (Handler.IsValid())
  {
    for (URPublisher *&Publisher : Publishers)
    {
      Publisher->Publish();
    }

    // for (auto &ActionServer : ActionServerList)
    // {
    //   ActionServer->Tick();
    // }
    Handler->Process();
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Handler Invalid"));
  }
}

void FRROSCommunicationContainer::DeInit()
{
  for (URPublisher *&Publisher : Publishers)
  {
    Publisher->DeInit();
  }
  if (Handler.IsValid())
  {
    Handler->Disconnect();
  }
}