#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAGoalSubscriber.h"
#include "control_msgs/FollowJointTrajectoryActionGoal.h"

DEFINE_LOG_CATEGORY_STATIC(LogRFJTAGoalSubscriber, Log, All)

URFJTAGoalSubscriber::URFJTAGoalSubscriber()
{
  MessageType = TEXT("control_msgs/FollowJointTrajectoryActionGoal");
}

void URFJTAGoalSubscriber::CreateSubscriber()
{
  if (Controller)
  {
    Subscriber = MakeShareable<FRFJTAGoalSubscriberCallback>(
        new FRFJTAGoalSubscriberCallback(Topic, MessageType, Controller));
  }
}

FRFJTAGoalSubscriberCallback::FRFJTAGoalSubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  JointTrajectoryController = Cast<URJointTrajectoryController>(InController);
}

TSharedPtr<FROSBridgeMsg> FRFJTAGoalSubscriberCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<control_msgs::FollowJointTrajectoryActionGoal> JointStateMessage =
      MakeShareable<control_msgs::FollowJointTrajectoryActionGoal>(new control_msgs::FollowJointTrajectoryActionGoal());

  JointStateMessage->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(JointStateMessage);
}

void FRFJTAGoalSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (JointTrajectoryController)
  {
    TSharedPtr<control_msgs::FollowJointTrajectoryActionGoal> TrajectoryMsg = StaticCastSharedPtr<control_msgs::FollowJointTrajectoryActionGoal>(Msg);

    TArray<FString> JointNames = TrajectoryMsg->GetGoal().GetTrajectory().GetJointNames();
    actionlib_msgs::GoalID Id = TrajectoryMsg->GetGoalId();
    FGoalStatusInfo GoalStatusInfo = FGoalStatusInfo(Id.GetId(), Id.GetStamp().Secs, Id.GetStamp().NSecs);

    UE_LOG(LogTemp, Log, TEXT("%s Recieved Trajectory Id: %s"), *FROSTime::Now().ToString(), *Id.GetId());

    FROSTime ActionStart = TrajectoryMsg->GetGoal().GetTrajectory().GetHeader().GetStamp();

    TArray<FTrajectoryPoints> Trajectory;
    for (const trajectory_msgs::JointTrajectoryPoint &JointPoint : TrajectoryMsg->GetGoal().GetTrajectory().GetPoints())
    {
      FROSTime TimeStep(JointPoint.GetTimeFromStart());
      Trajectory.Add(FTrajectoryPoints(TimeStep.Secs, TimeStep.NSecs, JointNames, JointPoint.GetPositions(), JointPoint.GetVelocities()));
    }

    // JointTrajectoryController->FollowJointTrajectory(ActionStart.GetTimeAsDouble(), GoalStatusInfo, Trajectory);

    double ActionTimeDiff = ActionStart.GetTimeAsDouble() - FROSTime::Now().GetTimeAsDouble();
    if(ActionTimeDiff <= 0.0f)
      {
        UE_LOG(LogTemp, Error, TEXT("Start Trajectory Instantly"));
        JointTrajectoryController->FollowJointTrajectory(ActionStart.GetTimeAsDouble(), GoalStatusInfo, Trajectory);
      }
    else
      {
        UE_LOG(LogTemp, Error, TEXT("Start Trajectory Delayed %f"), ActionTimeDiff);
        FTimerHandle MyTimerHandle;
        FTimerDelegate StartTrajectoryDelegate = FTimerDelegate::CreateUObject( JointTrajectoryController,  &URJointTrajectoryController::FollowJointTrajectory, ActionStart.GetTimeAsDouble(), GoalStatusInfo, Trajectory);
        // FTimerDelegate StartTrajectoryDelegate = FTimerDelegate::BindUFunction( JointTrajectoryController,  FName("FollowJointTrajectory"), ActionStart.GetTimeAsDouble(), GoalStatusInfo, Trajectory);

        // FTimerDelegate StartTrajectoryDelegate = FTimerDelegate::CreateUObject( JointController,  &URJointController::FollowTrajectory, ActionStart.GetTimeAsDouble(), GoalStatusInfo, Names, Trajectory);
        // FTimerDelegate StartTrajectoryDelegate = FTimerDelegate::BindUFunction( this,  FName("FollowTrajectory"), ActionStart.GetTimeAsDouble(), GoalStatusInfo, Names, Trajectory);
        JointTrajectoryController->GetOwner()->GetWorldTimerManager().SetTimer(MyTimerHandle, StartTrajectoryDelegate, ActionTimeDiff, false);
      }
  }
  else
  {
    UE_LOG(LogRFJTAGoalSubscriber, Error, TEXT("JointController not found"))
  }
  UE_LOG(LogRFJTAGoalSubscriber, Error, TEXT("GoalReceivedFinished"))
}
