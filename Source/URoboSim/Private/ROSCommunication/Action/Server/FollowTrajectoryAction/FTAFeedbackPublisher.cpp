
#include "ROSCommunication/Action/Server/FollowTrajectoryAction/FTAFeedbackPublisher.h"
#include "control_msgs/FollowJointTrajectoryActionFeedback.h"

URJointTrajectoryFeedbackPublisher::URJointTrajectoryFeedbackPublisher()
{
  JointParamTopic = TEXT("/whole_body_controller/joints");
}

void URJointTrajectoryFeedbackPublisher::SetMessageType()
{
  MessageType = TEXT("control_msgs/FollowJointTrajectoryActionFeedback");
}

void URJointTrajectoryFeedbackPublisher::SetOwner(UObject* InOwner)
{
  ARModel* Model =Cast<ARModel>(InOwner);
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Model->Plugins["ControllerComponent"]);
  Owner = Cast<URJointController>(ControllerComp->Controller.ControllerList["JointController"]);
  ConfigClient = NewObject<URJointStateConfigurationClient>(this);
  ConfigClient->JointParamTopic = JointParamTopic;
  // ConfigClient->URROSClient::Init(ControllerComp, Handler);
  ConfigClient->URROSClient::Init(InOwner, &Owner->TrajectoryStatus.JointNames, Handler);
}

void URJointTrajectoryFeedbackPublisher::Publish()
{
  // if(Owner->bFollowTrajectory)
  if(Owner->GetState() == UJointControllerState::FollowJointTrajectory)
    {
      TSharedPtr<control_msgs::FollowJointTrajectoryActionFeedback> Feedback =
        MakeShareable(new control_msgs::FollowJointTrajectoryActionFeedback());

      FGoalStatusInfo StatusInfo = Owner->GoalStatusList.Last();
      actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, "");
      Feedback->SetStatus(GS);
      Feedback->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

      control_msgs::FollowJointTrajectoryFeedback FeedbackMsg;
      FeedbackMsg.SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

      trajectory_msgs::JointTrajectoryPoint DesiredMsg;
      DesiredMsg.SetPositions(Owner->TrajectoryStatus.Desired);
      FeedbackMsg.SetDesired(DesiredMsg);

      trajectory_msgs::JointTrajectoryPoint ActualMsg;
      ActualMsg.SetPositions(Owner->TrajectoryStatus.Position);
      FeedbackMsg.SetActual(ActualMsg);

      trajectory_msgs::JointTrajectoryPoint ErrorMsg;
      ErrorMsg.SetPositions(Owner->TrajectoryStatus.Error);
      FeedbackMsg.SetError(ErrorMsg);

      FeedbackMsg.SetJointNames(Owner->TrajectoryStatus.JointNames);
      Feedback->SetFeedback(FeedbackMsg);
      Handler->PublishMsg(Topic, Feedback);
      Handler->Process();

      // UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
      Seq += 1;
    }
}
