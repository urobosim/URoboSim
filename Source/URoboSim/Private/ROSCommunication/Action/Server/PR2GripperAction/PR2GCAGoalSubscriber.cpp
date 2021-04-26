#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GCAGoalSubscriber.h"
#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GCASubscriberCallback.h"

void URPR2GripperCommandActionGoalSubscriber::SetMessageType()
{
  MessageType = TEXT("pr2_controllers_msgs/Pr2GripperCommandActionGoal");
}

void URPR2GripperCommandActionGoalSubscriber::CreateSubscriber()
{
  UE_LOG(LogTemp, Warning, TEXT("%s"), *ControllerName);

  Subscriber = MakeShareable<FROSGripperCommandSubscriberCallback>(
                                                                   new FROSGripperCommandSubscriberCallback(Topic, MessageType, ControllerComponent->GetController(ControllerName)));
  if(Subscriber.IsValid())
    {
      UE_LOG(LogTemp, Log, TEXT("Subscriber connected to RosBridge"));
    }
}
