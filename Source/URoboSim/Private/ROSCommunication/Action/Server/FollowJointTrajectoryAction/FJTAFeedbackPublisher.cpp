#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAFeedbackPublisher.h"
#include "control_msgs/FollowJointTrajectoryActionFeedback.h"

URFJTAFeedbackPublisher::URFJTAFeedbackPublisher()
{
  MessageType = TEXT("control_msgs/FollowJointTrajectoryActionFeedback");
  JointParamTopic = TEXT("/whole_body_controller/joints");
}

void URFJTAFeedbackPublisher::Init()
{
  Super::Init();
  if (GetOwner())
  {
    JointController = Cast<URJointController>(Controller);
    if (JointController)
    {
      ConfigClient = NewObject<URJointStateConfigurationClient>(GetOwner());
      ConfigClient->JointParamTopic = JointParamTopic;
      ConfigClient->Connect(Handler);
      ConfigClient->GetJointNames(&JointController->TrajectoryStatus.JointNames);
    }
  }
}

void URFJTAFeedbackPublisher::Publish()
{
  if (JointController && JointController->GetState() == UJointControllerState::FollowJointTrajectory)
  {
    static int Seq = 0;
    TSharedPtr<control_msgs::FollowJointTrajectoryActionFeedback> Feedback =
        MakeShareable(new control_msgs::FollowJointTrajectoryActionFeedback());

    FGoalStatusInfo StatusInfo = JointController->GoalStatusList.Last();
    actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, TEXT(""));
    Feedback->SetStatus(GS);
    Feedback->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

    control_msgs::FollowJointTrajectoryFeedback FeedbackMsg;
    FeedbackMsg.SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

    trajectory_msgs::JointTrajectoryPoint DesiredMsg;
    DesiredMsg.SetPositions(JointController->TrajectoryStatus.Desired);
    FeedbackMsg.SetDesired(DesiredMsg);

    trajectory_msgs::JointTrajectoryPoint ActualMsg;
    ActualMsg.SetPositions(JointController->TrajectoryStatus.Position);
    FeedbackMsg.SetActual(ActualMsg);

    trajectory_msgs::JointTrajectoryPoint ErrorMsg;
    ErrorMsg.SetPositions(JointController->TrajectoryStatus.Error);
    FeedbackMsg.SetError(ErrorMsg);

    FeedbackMsg.SetJointNames(JointController->TrajectoryStatus.JointNames);
    Feedback->SetFeedback(FeedbackMsg);
    Handler->PublishMsg(Topic, Feedback);
    Handler->Process();

    // UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
    Seq++;
  }
}
