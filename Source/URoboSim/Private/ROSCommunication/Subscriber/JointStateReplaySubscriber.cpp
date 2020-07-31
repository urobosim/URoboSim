
#include "ROSCommunication/Subscriber/JointStateReplaySubscriber.h"
#include "ROSCommunication/Subscriber/JointStateReplaySubscriberCallback.h"

void URJointStateReplaySubscriber::SetMessageType()
{
  MessageType = TEXT("sensor_msgs/JointState");
}

void URJointStateReplaySubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FROSJointStateReplaySubscriberCallback>(
                                                                     new FROSJointStateReplaySubscriberCallback(Topic, MessageType,  ControllerComponent->Controller.ControllerList[ControllerName]));
}
