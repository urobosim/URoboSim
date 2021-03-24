#include "ROSCommunication/Subscriber/RSubscriber.h"

DEFINE_LOG_CATEGORY_STATIC(LogRSubscriber, Log, All)

void URSubscriber::Init(UObject* InModel, TSharedPtr<FROSBridgeHandler> InHandler, FString InTopic )
{
  SetOwner(InModel);
  Handler = InHandler;
  if(!InTopic.Equals(""))
  {
    Topic = InTopic;
  }
  SetMessageType();
  CreateSubscriber();
}

void URSubscriber::CreateSubscriber()
{
  if (Subscriber.IsValid())
  {
    UE_LOG(LogRSubscriber, Log, TEXT("%s is connected to ROSBridge"), *GetName())
    Handler->AddSubscriber(Subscriber);
  }
}