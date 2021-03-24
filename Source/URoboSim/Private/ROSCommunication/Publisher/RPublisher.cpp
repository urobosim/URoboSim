#include "ROSCommunication/Publisher/RPublisher.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPublisher, Log, All)

void URPublisher::DeInit()
{
  Handler->Disconnect();
}

void URPublisher::Init(UObject *InOwner, const FString &InHostIp, const uint32 &InPort)
{
  SetOwner(InOwner);
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(InHostIp, InPort));
  Handler->Connect();
  SetMessageType();
  CreatePublisher();
}

void URPublisher::Init(UObject *InOwner, const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InTopic)
{
  SetOwner(InOwner);
  Handler = InHandler;
  if (!InTopic.Equals(""))
  {
    Topic = InTopic;
  }
  SetMessageType();
  CreatePublisher();
}

void URPublisher::CreatePublisher()
{
  Publisher = MakeShareable<FROSBridgePublisher>(new FROSBridgePublisher(Topic, MessageType));
  if (Publisher.IsValid())
  {
    UE_LOG(LogRPublisher, Log, TEXT("%s is connected to ROSBridge"), *GetName())
    Handler->AddPublisher(Publisher);
  }
}