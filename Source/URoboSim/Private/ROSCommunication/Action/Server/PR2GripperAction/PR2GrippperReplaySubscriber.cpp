#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GripperReplaySubscriber.h"
#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GripperReplaySubscriberCallback.h"

void URPR2GripperReplaySubscriber::SetMessageType()
{
  MessageType = TEXT("pr2_controllers_msgs/Pr2GripperCommandActionGoal");
}

void URPR2GripperReplaySubscriber::CreateSubscriber()
{
  UE_LOG(LogTemp, Warning, TEXT("%s"), *ControllerName);

  Subscriber = MakeShareable<FROSGripperReplaySubscriberCallback>(
                                                                   new FROSGripperReplaySubscriberCallback(Topic, MessageType, ControllerComponent->GetController(ControllerName)));
  if(Subscriber.IsValid())
    {
      UE_LOG(LogTemp, Log, TEXT("Subscriber connected to RosBridge"));
    }
}
