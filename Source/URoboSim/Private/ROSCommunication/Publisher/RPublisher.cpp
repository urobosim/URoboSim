#include "ROSCommunication/Publisher/RPublisher.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPublisher, Log, All)

void URPublisher::Init(const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InTopic)
{
  Handler = InHandler;
  if (Handler.IsValid())
  {
    Init(InTopic);
  }
  else
  {
    UE_LOG(LogRPublisher, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URPublisher::Init(const FString &WebsocketIPAddr, const uint32 &WebsocketPort, const FString &InTopic)
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  if (Handler.IsValid())
  {
    Handler->Connect();
    Init(InTopic);
  }
  else
  {
    UE_LOG(LogRPublisher, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URPublisher::Init(const FString &InTopic)
{
  if (!InTopic.Equals(""))
  {
    Topic = InTopic;
  }
  Init();
  CreatePublisher();
}

void URPublisher::Init()
{
  if (!Owner && Cast<ARModel>(GetOuter()))
  {
    Owner = Cast<ARModel>(GetOuter());
  }
}

void URPublisher::DeInit()
{
  if (Handler.IsValid())
  {
    Handler->Disconnect();
  }
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
  Publisher = MakeShareable<FROSBridgePublisher>(new FROSBridgePublisher(Topic, MessageType));
  if (Publisher.IsValid())
  {
    Handler->AddPublisher(Publisher);
  }
}

void URPublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  if (PublisherParameters)
  {
    Topic = PublisherParameters->Topic;
    MessageType = PublisherParameters->MessageType;
  }
}