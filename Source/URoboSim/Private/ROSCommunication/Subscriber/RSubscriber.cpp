#include "ROSCommunication/Subscriber/RSubscriber.h"

DEFINE_LOG_CATEGORY_STATIC(LogRSubscriber, Log, All)

void URSubscriber::Init(UObject *InOwner, const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InTopic)
{
  Handler = InHandler;
  if (Handler.IsValid())
  {
    Init(InOwner, InTopic);
  }
  else
  {
    UE_LOG(LogRSubscriber, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URSubscriber::Init(UObject *InOwner, const FString &WebsocketIPAddr, const uint32 &WebsocketPort, const FString &InTopic)
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  if (Handler.IsValid())
  {
    Handler->Connect();
    Init(InOwner, InTopic);
  }
  else
  {
    UE_LOG(LogRSubscriber, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URSubscriber::Init(UObject *InOwner, const FString &InTopic)
{
  if (!SubscriberParameters)
  {
    SubscriberParameters = CreateDefaultSubobject<URSubscriberParameter>(TEXT("SubscriberParameters"));
  }

  SetOwner(InOwner);
  if (!InTopic.Equals(""))
  {
    SubscriberParameters->Topic = InTopic;
  }
  Init();
  CreateSubscriber();
  AddSubscriber();
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
    UE_LOG(LogRSubscriber, Log, TEXT("%s is connected to ROSBridge"), *GetName())
    Handler->AddSubscriber(Subscriber);
  }
}