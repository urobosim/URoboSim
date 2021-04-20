#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "ROSCommunication/RROSCommunicationComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRSubscriber, Log, All)

void URSubscriber::Init(const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InTopic)
{
  Handler = InHandler;
  if (Handler.IsValid())
  {
    Init(InTopic);
  }
  else
  {
    UE_LOG(LogRSubscriber, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URSubscriber::Init(const FString &WebsocketIPAddr, const uint32 &WebsocketPort, const FString &InTopic)
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  if (Handler.IsValid())
  {
    Handler->Connect();
    UE_LOG(LogRSubscriber, Log, TEXT("%s is connected to ROSBridge"), *GetName())
    Init(InTopic);
  }
  else
  {
    UE_LOG(LogRSubscriber, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URSubscriber::Init(const FString &InTopic)
{
  if (!InTopic.Equals(""))
  {
    Topic = InTopic;
  }
  Init();
  CreateSubscriber();
  AddSubscriber();
}

void URSubscriber::Init()
{
  SetOwner();
}

void URSubscriber::SetOwner()
{
  if (!Owner)
  {
    if (Cast<ARModel>(GetOuter()))
    {
      Owner = Cast<ARModel>(GetOuter());
    }
    else if (Cast<URROSCommunicationComponent>(GetOuter()) && Cast<ARModel>(Cast<URROSCommunicationComponent>(GetOuter())->GetOwner()))
    {
      Owner = Cast<ARModel>(Cast<URROSCommunicationComponent>(GetOuter())->GetOwner());
    }
  }
  if (!Owner)
  {
    UE_LOG(LogRSubscriber, Error, TEXT("Owner of %s not found, Outer is %s"), *GetName(), *GetOuter()->GetName())
  }
}

void URSubscriber::DeInit()
{
  if (Handler.IsValid())
  {
    Handler->Disconnect();
  }
}

void URSubscriber::Tick()
{
  if (Handler.IsValid())
  {
    Handler->Process();
  }
}

void URSubscriber::AddSubscriber()
{
  if (Subscriber.IsValid())
  {
    Handler->AddSubscriber(Subscriber);
  }
}

void URSubscriber::SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters)
{
  if (SubscriberParameters)
  {
    Topic = SubscriberParameters->Topic;
    MessageType = SubscriberParameters->MessageType;
  }
}