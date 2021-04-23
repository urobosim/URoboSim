#include "ROSCommunication/Action/Server/ActionCancelSubscriber.h"
#include "ROSCommunication/Action/Server/ActionCancelCallback.h"

void URActionCancelSubscriber::SetMessageType()
{
  MessageType = TEXT("actionlib_msgs/GoalID");
}

void URActionCancelSubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FROSActionCancelCallback>(
                                                       new FROSActionCancelCallback(Topic, MessageType, ControllerComponent->Controller.ControllerList[ControllerName]));
  if(Subscriber.IsValid())
    {
      UE_LOG(LogTemp, Log, TEXT("GiskardSubscriber connected to RosBridge"));
    }
}
