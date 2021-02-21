
#include "ROSCommunication/Subscriber/JointStateSubscriber.h"
#include "ROSCommunication/Subscriber/RROSJointStateSubscriberCallback.h"

void URJointStateSubscriber::SetMessageType()
{
  MessageType = TEXT("sensor_msgs/JointState");
}

void URJointStateSubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FROSJointStateSubscriberCallback>(
                new FROSJointStateSubscriberCallback(Topic, TEXT("sensor_msgs/JointState"), Model));
  if(Subscriber.IsValid())
    {
      UE_LOG(LogTemp, Log, TEXT("Subscriber connected to RosBridge"));
    }
}
