#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAGoalSubscriber.h"
#include "control_msgs/FollowJointTrajectoryActionGoal.h"

DEFINE_LOG_CATEGORY_STATIC(LogRFJTAGoalSubscriber, Log, All)

URFJTAGoalSubscriber::URFJTAGoalSubscriber()
{
  MessageType = TEXT("control_msgs/FollowJointTrajectoryActionGoal");
  JointControllerName = TEXT("JointController");
}

void URFJTAGoalSubscriber::CreateSubscriber()
{
  URControllerComponent *ControllerComponent = Cast<URControllerComponent>(GetOwner()->GetPlugin(TEXT("ControllerComponent")));
  if (ControllerComponent)
  {
    Subscriber = MakeShareable<FRFJTAGoalSubscriberCallback>(
        new FRFJTAGoalSubscriberCallback(Topic, MessageType, ControllerComponent->GetController(JointControllerName)));
  }
}

FRFJTAGoalSubscriberCallback::FRFJTAGoalSubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  JointController = Cast<URJointController>(InController);
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
  if (JointController)
  {
    TSharedPtr<control_msgs::FollowJointTrajectoryActionGoal> TrajectoryMsg = StaticCastSharedPtr<control_msgs::FollowJointTrajectoryActionGoal>(Msg);

    TArray<FString> JointNames = TrajectoryMsg->GetGoal().GetTrajectory().GetJointNames();
    JointController->SetJointNames(JointNames);

    actionlib_msgs::GoalID GoalID = TrajectoryMsg->GetGoalId();
    UE_LOG(LogRFJTAGoalSubscriber, Log, TEXT("%s Received Trajectory Goal ID: %s"), *FROSTime::Now().ToString(), *GoalID.GetId());

    JointController->AddGoalStatus(FGoalStatusInfo(GoalID.GetId(), GoalID.GetStamp().Secs, GoalID.GetStamp().NSecs));

    FROSTime ActionStart = TrajectoryMsg->GetGoal().GetTrajectory().GetHeader().GetStamp();

    for (const trajectory_msgs::JointTrajectoryPoint &JointPoint : TrajectoryMsg->GetGoal().GetTrajectory().GetPoints())
    {
      FROSTime TimeStep(JointPoint.GetTimeFromStart());
      JointController->DesiredTrajectory.Add(FTrajectoryPoints(TimeStep.Secs, TimeStep.NSecs, JointNames, JointPoint.GetPositions(), JointPoint.GetVelocities()));
    }

    double ActionTimeDelay = ActionStart.GetTimeAsDouble() - FROSTime::Now().GetTimeAsDouble();
    if (ActionTimeDelay > 0.)
    {
      FTimerHandle MyTimerHandle;
      JointController->GetOwner()->GetWorldTimerManager().SetTimer(MyTimerHandle, JointController, &URJointController::FollowJointTrajectory, ActionTimeDelay, false);
    }
    else
    {
      JointController->FollowJointTrajectory();
    }
  }
  else
  {
    UE_LOG(LogRFJTAGoalSubscriber, Error, TEXT("JointController not found"));
  }
}