
#include "ROSCommunication/JointTrajectoryControllerStatePublisher.h"

URJointTrajectoryControllerStatePublisher::URJointTrajectoryControllerStatePublisher()
{
  Topic = TEXT("/whole_body_controller/body/state");
}

void URJointTrajectoryControllerStatePublisher::SetMessageType()
{
  MessageType = TEXT("control_msgs/JointTrajectoryControllerState");
}

void URJointTrajectoryControllerStatePublisher::Publish()
{
  TSharedPtr<control_msgs::FollowJointTrajectoryFeedback> Feedback =
    MakeShareable(new control_msgs::FollowJointTrajectoryFeedback());

  Feedback->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));


  trajectory_msgs::JointTrajectoryPoint DesiredMsg;
  DesiredMsg.SetPositions(Owner->TrajectoryStatus.Desired);
  Feedback->SetDesired(DesiredMsg);

  trajectory_msgs::JointTrajectoryPoint ActualMsg;
  ActualMsg.SetPositions(Owner->TrajectoryStatus.Position);
  Feedback->SetActual(ActualMsg);

  trajectory_msgs::JointTrajectoryPoint ErrorMsg;
  ErrorMsg.SetPositions(Owner->TrajectoryStatus.Error);
  Feedback->SetError(ErrorMsg);

  Feedback->SetJointNames(Owner->TrajectoryStatus.JointNames);
  Handler->PublishMsg(Topic, Feedback);
  Handler->Process();

  Seq += 1;
}
