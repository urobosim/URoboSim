#include "ROSCommunication/Subscriber/RSubscriber.h"

DEFINE_LOG_CATEGORY_STATIC(LogRSubscriber, Log, All)

void USubscriber::SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters)
{
  if (SubscriberParameters)
  {
    Topic = SubscriberParameters->Topic;
    MessageType = SubscriberParameters->MessageType;
  }
}

void USubscriber::Init()
{
  CreateSubscriber();
  if (Subscriber.IsValid())
  {
    Handler->AddSubscriber(Subscriber);
  }
}

void USubscriber::Tick()
{
  if (Handler.IsValid())
  {
    Handler->Process();
  }
}

void URSubscriber::Init()
{
  SetOwner(GetOuter());
  Super::Init();
}
