#include "ROSCommunication/Actions/PerceiveObjectAction/POAGoalSubscriber.h"
#include "ROSCommunication/Subscriber/RROSSubscriberCallback.h"

void URPerceiveObjectActionGoalSubscriber::SetMessageType()
{
  MessageType = TEXT("urobosim_msgs/PerceiveObjectActionGoal");
}

void URPerceiveObjectActionGoalSubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FROSPerceiveObjectGoalSubscriberCallback>(
                                                                       new FROSPerceiveObjectGoalSubscriberCallback(Topic, MessageType, ControllerComponent->Controller.ControllerList["CameraController"]));
  if(Subscriber.IsValid())
    {
      UE_LOG(LogTemp, Log, TEXT("Subscriber connected to RosBridge"));
    }
}
