#include "ROSCommunication/Publisher/RPublisher.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPublisher, Log, All)

void URPublisher::DeInit()
{
  Handler->Disconnect();
}

void URPublisher::Init(UObject *InOwner, const FString &InHostIp, const uint32 &InPort)
{
  Seq = 0;
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(InHostIp, InPort));
  Handler->Connect();
  SetMessageType();
  SetOwner(InOwner);
  CreatePublisher();
  Handler->AddPublisher(Publisher);
}

void URPublisher::Init(UObject *InOwner, const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InTopic)
{
  Seq = 0;
  Handler = InHandler;
  if (!InTopic.Equals(""))
  {
    Topic = InTopic;
  }
  SetMessageType();
  SetOwner(InOwner);
  CreatePublisher();
  Handler->AddPublisher(Publisher);
}

void URPublisher::CreatePublisher()
{
  Publisher = MakeShareable<FROSBridgePublisher>(new FROSBridgePublisher(Topic, MessageType));

  if (Publisher.IsValid())
  {
    UE_LOG(LogRPublisher, Log, TEXT("%s is connected to ROSBridge"), *GetName())
  }
}