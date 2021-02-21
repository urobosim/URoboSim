
#include "ROSCommunication/Subscriber/BaseControllerCallback.h"
#include "control_msgs/FollowJointTrajectoryActionGoal.h"

FROSBaseControllerCallback::FROSBaseControllerCallback(
	const FString& InTopic, const FString& InType, UObject* InController) :
	FROSBridgeSubscriber(InTopic, InType)
{
	BaseController = Cast<URBaseController>(InController);
}

FROSBaseControllerCallback::~FROSBaseControllerCallback()
{
}

TSharedPtr<FROSBridgeMsg> FROSBaseControllerCallback::ParseMessage
(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<control_msgs::FollowJointTrajectoryActionGoal> JointStateMessage =
    MakeShareable<control_msgs::FollowJointTrajectoryActionGoal>(new control_msgs::FollowJointTrajectoryActionGoal());

  JointStateMessage->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(JointStateMessage);
}

void FROSBaseControllerCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
	// if(BaseController)
	// {
	// 	TSharedPtr<control_msgs::FollowJointTrajectoryActionGoal> TrajectoryMsg = StaticCastSharedPtr<control_msgs::FollowJointTrajectoryActionGoal>(Msg);
    //
	// 	TArray<FString> Names =TrajectoryMsg->GetGoal().GetTrajectory().GetJointNames();
	// 	BaseController->SetJointNames(Names);
	// 	for(auto& JointPoint : TrajectoryMsg->GetGoal().GetTrajectory().GetPointss())
	// 	{
	// 		FTrajectoryPoints TempPoints;
	// 		TempPoints.Points = JointPoint.GetPositions();
	// 		BaseController->Trajectory.Add(TempPoints) ;
	// 	}
	// 	BaseController->bFollowTrajectory = true;
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("JonintController not found"));
}
