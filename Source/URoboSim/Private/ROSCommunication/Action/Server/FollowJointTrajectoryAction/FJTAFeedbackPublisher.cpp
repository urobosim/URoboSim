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
    JointTrajectoryController = Cast<URJointTrajectoryController>(Controller);
    // if (JointTrajectoryController)
    // {
    //   ConfigClient = NewObject<URJointStateConfigurationClient>(GetOwner());
    //   ConfigClient->JointParamTopic = JointParamTopic;
    //   ConfigClient->Connect(Handler);
    //   ConfigClient->GetJointNames([this](const TArray<FString> &JointNames){ JointTrajectoryController->SetJointNames(JointNames); });
    // }
  }
}

void URFJTAFeedbackPublisher::Publish()
{
  if (JointTrajectoryController && JointTrajectoryController->GetState() == UJointControllerState::FollowJointTrajectory)
  {
    static int Seq = 0;
    TSharedPtr<control_msgs::FollowJointTrajectoryActionFeedback> Feedback =
        MakeShareable(new control_msgs::FollowJointTrajectoryActionFeedback());

    FGoalStatusInfo StatusInfo = JointTrajectoryController->GetGoalStatusList().Last();
    actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, TEXT(""));
    Feedback->SetStatus(GS);
    Feedback->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

    TArray<FString> JointNames;
    TArray<double> DesiredPositions;
    TArray<double> CurrentPositions;
    TArray<double> ErrorPositions;
    TArray<double> DesiredVelocities;
    TArray<double> CurrentVelocities;
    TArray<double> ErrorVelocities;
    for (const TPair<FString, FJointState> &DesiredJointState : JointTrajectoryController->DesiredJointStates)
    {
      if (URJoint *Joint = GetOwner()->GetJoint(DesiredJointState.Key))
      {
        JointNames.Add(Joint->GetName());

        DesiredPositions.Add(DesiredJointState.Value.JointPosition);
        DesiredVelocities.Add(DesiredJointState.Value.JointVelocity);

        CurrentPositions.Add(Joint->GetJointState().JointPosition);
        CurrentVelocities.Add(Joint->GetJointState().JointVelocity);

        ErrorPositions.Add(DesiredJointState.Value.JointPosition - Joint->GetJointState().JointPosition);
        ErrorVelocities.Add(DesiredJointState.Value.JointVelocity - Joint->GetJointState().JointVelocity);
      }
    }

    control_msgs::FollowJointTrajectoryFeedback FeedbackMsg;
    FeedbackMsg.SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));
    FeedbackMsg.SetJointNames(JointNames);

    trajectory_msgs::JointTrajectoryPoint DesiredStatesMsg;
    DesiredStatesMsg.SetPositions(DesiredPositions);
    DesiredStatesMsg.SetVelocities(DesiredVelocities);
    FeedbackMsg.SetDesired(DesiredStatesMsg);

    trajectory_msgs::JointTrajectoryPoint CurrentStatesMsg;
    CurrentStatesMsg.SetPositions(CurrentPositions);
    CurrentStatesMsg.SetVelocities(CurrentVelocities);
    FeedbackMsg.SetActual(CurrentStatesMsg);

    trajectory_msgs::JointTrajectoryPoint ErrorStatesMsg;
    ErrorStatesMsg.SetPositions(ErrorPositions);
    ErrorStatesMsg.SetVelocities(ErrorVelocities);
    FeedbackMsg.SetError(ErrorStatesMsg);

    Feedback->SetFeedback(FeedbackMsg);

    Handler->PublishMsg(Topic, Feedback);
    Handler->Process();

    // UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
    Seq++;
  }
}
