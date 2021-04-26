
#include "ROSCommunication/Subscriber/VelocityCommandSubscriber.h"
#include "ROSCommunication/Subscriber/VelocityCommandSubscriberCallback.h"

void URVelocityCommandSubscriber::SetMessageType()
{
  MessageType = TEXT("geometry_msgs/Twist");
  Topic = TEXT("/base_controller/command");
}

void URVelocityCommandSubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FROSVelocityCommandSubscriberCallback>(
                                                                    new FROSVelocityCommandSubscriberCallback(Topic, MessageType, ControllerComponent->GetController(TEXT("BaseController"))));
  if(Subscriber.IsValid())
    {
      UE_LOG(LogTemp, Log, TEXT("Subscriber connected to RosBridge"));
    }
}
