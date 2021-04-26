#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAGoalSubscriber.h"
#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAGoalSubscriberCallback.h"

void URPerceiveObjectActionGoalSubscriber::SetMessageType()
{
  MessageType = TEXT("urobosim_msgs/PerceiveObjectActionGoal");
}

void URPerceiveObjectActionGoalSubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FROSPerceiveObjectGoalSubscriberCallback>(
                                                                       new FROSPerceiveObjectGoalSubscriberCallback(Topic, MessageType, ControllerComponent->GetController(TEXT("CameraController"))));
  if(Subscriber.IsValid())
    {
      UE_LOG(LogTemp, Log, TEXT("Subscriber connected to RosBridge"));
    }
}
