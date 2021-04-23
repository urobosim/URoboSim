#include "ROSCommunication/Action/Server/PR2HeadAction/PR2HAGoalSubscriber.h"
#include "ROSCommunication/Action/Server/PR2HeadAction/PR2HAGoalSubscriberCallback.h"

void URPR2HeadActionGoalSubscriber::SetMessageType()
{
  MessageType = TEXT("pr2_controllers_msgs/PointHeadActionGoal");
}

void URPR2HeadActionGoalSubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FROSPointHeadGoalSubscriberCallback>(
                                                                  new FROSPointHeadGoalSubscriberCallback(Topic, MessageType, ControllerComponent->Controller.ControllerList["HeadController"]));
  if(Subscriber.IsValid())
    {
      UE_LOG(LogTemp, Log, TEXT("Subscriber connected to RosBridge"));
    }
}
