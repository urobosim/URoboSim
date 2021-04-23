#include "ROSCommunication/Action/Server/FollowTrajectoryAction/FTACancelCallback.h"

FROSFollowTrajectoryActionCancelCallback::FROSFollowTrajectoryActionCancelCallback(
	const FString& InTopic, const FString& InType, UObject* InController) :
	FROSBridgeSubscriber(InTopic, InType)
{
	JointController = Cast<URJointController>(InController);
}

FROSFollowTrajectoryActionCancelCallback::~FROSFollowTrajectoryActionCancelCallback()
{
}

void FROSFollowTrajectoryActionCancelCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if(JointController)
    {
      UE_LOG(LogTemp, Error, TEXT("The FollowJointTrajectoryActionCancel Callback is a dummy and not implemented yet"));
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("JonintController not found"));
    }
}

TSharedPtr<FROSBridgeMsg> FROSFollowTrajectoryActionCancelCallback::ParseMessage
(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<actionlib_msgs::GoalID> GoalId =
    MakeShareable<actionlib_msgs::GoalID>(new actionlib_msgs::GoalID());

  GoalId->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(GoalId);
}
