#include "ROSCommunication/Actions/FollowTrajectoryAction/FTAGoalSubscriber.h"
#include "ROSCommunication/Subscriber/RROSSubscriberCallback.h"

void URFollowJointTrajectoryActionGoalSubscriber::SetMessageType()
{
  MessageType = TEXT("control_msgs/FollowJointTrajectoryActionGoal");
}

void URFollowJointTrajectoryActionGoalSubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FROSWholeBodyControllerCallback>(
                                                              new FROSWholeBodyControllerCallback(Topic, MessageType, ControllerComponent->Controller.ControllerList["JointController"]));
  if(Subscriber.IsValid())
    {
      UE_LOG(LogTemp, Log, TEXT("GiskardSubscriber connected to RosBridge"));
    }
}
