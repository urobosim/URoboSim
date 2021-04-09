#include "ROSCommunication/Publisher/RPublisher.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPublisher, Log, All)

void URPublisher::Init(UObject *InOwner, const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InTopic)
{
  Handler = InHandler;
  if (Handler.IsValid())
  {
    Init(InOwner, InTopic);
  }
  else
  {
    UE_LOG(LogRPublisher, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URPublisher::Init(UObject *InOwner, const FString &WebsocketIPAddr, const uint32 &WebsocketPort, const FString &InTopic)
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  if (Handler.IsValid())
  {
    Handler->Connect();
    Init(InOwner, InTopic);
  }
  else
  {
    UE_LOG(LogRPublisher, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URPublisher::Init(UObject *InOwner, const FString &InTopic)
{
  if (!PublisherParameters)
  {
    PublisherParameters = CreateDefaultSubobject<URPublisherParameter>(TEXT("PublisherParameters"));
  }
  
  SetOwner(InOwner);
  if (!InTopic.Equals(""))
  {
    PublisherParameters->Topic = InTopic;
  }
  Init();
  CreatePublisher();
}

void URPublisher::Tick()
{
  if (Handler.IsValid())
  {
    Publish();
  }
}

void URPublisher::CreatePublisher()
{
  Publisher = MakeShareable<FROSBridgePublisher>(new FROSBridgePublisher(PublisherParameters->Topic, PublisherParameters->MessageType));
  if (Publisher.IsValid())
  {
    Handler->AddPublisher(Publisher);
  }
}