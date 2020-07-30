#include "ROSCommunication/Subscriber/OdometrieReplaySubscriber.h"
#include "ROSCommunication/Subscriber/RROSSubscriberCallback.h"

void UROdometrieReplaySubscriber::SetMessageType()
{
  MessageType = TEXT("tf2_msgs/TFMessage");
}

void UROdometrieReplaySubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FROSOdomReplaySubscriberCallback>(
                                                               new FROSOdomReplaySubscriberCallback(Topic, MessageType,  ControllerComponent->Controller.ControllerList[ControllerName]));
}
