#include "ROSCommunication/Publisher/JointTrajectoryControllerStatePublisher.h"
#include "control_msgs/JointTrajectoryControllerState.h"

URJointTrajectoryControllerStatePublisher::URJointTrajectoryControllerStatePublisher()
{
  Topic = TEXT("/whole_body_controller/body/state");
  MessageType = TEXT("control_msgs/JointTrajectoryControllerState");
  JointParamTopic = TEXT("/whole_body_controller/joints");
  JointControllerName = TEXT("JointController");
}

void URJointTrajectoryControllerStatePublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  if (URJointTrajectoryControllerStatePublisherParameter *JointTrajectoryControllerStatePublisherParameters = Cast<URJointTrajectoryControllerStatePublisherParameter>(PublisherParameters))
  {
    Super::SetPublishParameters(PublisherParameters);
    FrameId = JointTrajectoryControllerStatePublisherParameters->FrameId;
    JointParamTopic = JointTrajectoryControllerStatePublisherParameters->JointParamTopic;
    JointControllerName = JointTrajectoryControllerStatePublisherParameters->JointControllerName;
  }  
}

void URJointTrajectoryControllerStatePublisher::Init()
{
  Super::Init();
  if (GetOwner())
  {
    JointController = Cast<URJointController>(GetOwner()->GetController(JointControllerName));
    if (JointController)
    {
      ConfigClient = NewObject<URJointStateConfigurationClient>(GetOwner());
      ConfigClient->JointParamTopic = JointParamTopic;
      ConfigClient->Connect(Handler);
      ConfigClient->GetJointNames(&JointController->TrajectoryStatus.JointNames);
    }
  }
}

void URJointTrajectoryControllerStatePublisher::Publish()
{
  if (GetOwner() && JointController)
  {
    static int Seq = 0;
    
    TSharedPtr<control_msgs::JointTrajectoryControllerState> State =
        MakeShareable(new control_msgs::JointTrajectoryControllerState());

    State->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

    State->SetJointNames(JointController->TrajectoryStatus.JointNames);

    trajectory_msgs::JointTrajectoryPoint DesiredMsg;
    DesiredMsg.SetPositions(JointController->TrajectoryStatus.Desired);
    State->SetDesired(DesiredMsg);

    trajectory_msgs::JointTrajectoryPoint ActualMsg;
    ActualMsg.SetPositions(JointController->TrajectoryStatus.Position);
    State->SetActual(ActualMsg);

    trajectory_msgs::JointTrajectoryPoint ErrorMsg;
    ErrorMsg.SetPositions(JointController->TrajectoryStatus.Error);
    State->SetError(ErrorMsg);

    State->SetJointNames(JointController->TrajectoryStatus.JointNames);
    
    Handler->PublishMsg(Topic, State);
    Handler->Process();

    Seq++;
  }
}
