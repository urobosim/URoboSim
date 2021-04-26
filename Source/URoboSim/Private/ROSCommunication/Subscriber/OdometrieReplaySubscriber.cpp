#include "ROSCommunication/Subscriber/OdometrieReplaySubscriber.h"
#include "ROSCommunication/Subscriber/OdomReplaySubscriberCallback.h"

void UROdometrieReplaySubscriber::SetMessageType()
{
  MessageType = TEXT("tf2_msgs/TFMessage");
}

void UROdometrieReplaySubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FROSOdomReplaySubscriberCallback>(
                                                               new FROSOdomReplaySubscriberCallback(Topic, MessageType,  ControllerComponent->GetController(ControllerName)));
}
