#include "ROSCommunication/Action/Server/FollowTrajectoryAction/FTAGoalSubscriber.h"
#include "ROSCommunication/Action/Server/FollowTrajectoryAction/WholeBodyControllerCallback.h"

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
