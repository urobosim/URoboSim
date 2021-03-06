#include "ROSCommunication/Actions/FollowTrajectoryAction/WholeBodyControllerCallback.h"
#include "control_msgs/FollowJointTrajectoryActionGoal.h"

FROSWholeBodyControllerCallback::FROSWholeBodyControllerCallback(
	const FString& InTopic, const FString& InType, UObject* InController) :
	FROSBridgeSubscriber(InTopic, InType)
{
  JointController = Cast<URJointController>(InController);
}

FROSWholeBodyControllerCallback::~FROSWholeBodyControllerCallback()
{
}

TSharedPtr<FROSBridgeMsg> FROSWholeBodyControllerCallback::ParseMessage
(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<control_msgs::FollowJointTrajectoryActionGoal> JointStateMessage =
    MakeShareable<control_msgs::FollowJointTrajectoryActionGoal>(new control_msgs::FollowJointTrajectoryActionGoal());

  JointStateMessage->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(JointStateMessage);
}

void FROSWholeBodyControllerCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if(JointController)
    {
      TSharedPtr<control_msgs::FollowJointTrajectoryActionGoal> TrajectoryMsg = StaticCastSharedPtr<control_msgs::FollowJointTrajectoryActionGoal>(Msg);

      TArray<FString> Names =TrajectoryMsg->GetGoal().GetTrajectory().GetJointNames();
      JointController->SetJointNames(Names);

      actionlib_msgs::GoalID Id = TrajectoryMsg->GetGoalId();
      JointController->GoalStatusList.Add(FGoalStatusInfo(Id.GetId(), Id.GetStamp().Secs, Id.GetStamp().NSecs));
      UE_LOG(LogTemp, Log, TEXT("Recieved Trajectory Id: %s"), *Id.GetId());
      for(auto& JointPoint : TrajectoryMsg->GetGoal().GetTrajectory().GetPoints())
        {
          FTrajectoryPoints TempPoints;
          // FTrajectoryPoints TempError;
          TempPoints.Points = JointPoint.GetPositions();
          TempPoints.Velocities = JointPoint.GetVelocities();
          FROSTime TimeStep(JointPoint.GetTimeFromStart());
          TempPoints.SetTimeStep(TimeStep.Secs, TimeStep.NSecs);
          JointController->Trajectory.Add(TempPoints) ;
          // JointController->Error.Add(TempError) ;
        }
      // JointController->bFollowTrajectory = true;
      JointController->FollowTrajectory();
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("JonintController not found"));
    }
}
