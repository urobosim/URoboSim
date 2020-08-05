#include "ROSCommunication/Actions/ActionCancelSubscriber.h"
#include "ROSCommunication/Actions/ActionCancelCallback.h"

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
