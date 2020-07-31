#include "ROSCommunication/Subscriber/TFSubscriber.h"
#include "ROSCommunication/Subscriber/TFSubscriber.h"

void URTFSubscriber::SetMessageType()
{
  MessageType = TEXT("tf2_msgs/TFMessage");
}

void URTFSubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FROSTFReplaySubscriberCallback>(
                                                             new FROSTFReplaySubscriberCallback(Topic, MessageType,  ControllerComponent->Controller.ControllerList[ControllerName]));
}
