#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/RROSCommunicationComponent.h"

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
    UE_LOG(LogRPublisher, Log, TEXT("%s is connected to ROSBridge"), *GetName())
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
  SetOwner();
}

void URPublisher::SetOwner()
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
    UE_LOG(LogRPublisher, Error, TEXT("Owner of %s not found, Outer is %s"), *GetName(), *GetOuter()->GetName())
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