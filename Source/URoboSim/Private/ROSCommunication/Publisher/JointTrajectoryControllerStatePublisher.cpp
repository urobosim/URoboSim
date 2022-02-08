#include "ROSCommunication/Publisher/JointTrajectoryControllerStatePublisher.h"
#include "control_msgs/JointTrajectoryControllerState.h"

URJointTrajectoryControllerStatePublisher::URJointTrajectoryControllerStatePublisher()
{
  Topic = TEXT("/whole_body_controller/body/state");
  MessageType = TEXT("control_msgs/JointTrajectoryControllerState");
  JointParamTopic = TEXT("/whole_body_controller/joints");
  JointTrajectoryControllerName = TEXT("JointTrajectoryController");
}

void URJointTrajectoryControllerStatePublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  if (URJointTrajectoryControllerStatePublisherParameter *JointTrajectoryControllerStatePublisherParameters = Cast<URJointTrajectoryControllerStatePublisherParameter>(PublisherParameters))
  {
    Super::SetPublishParameters(PublisherParameters);
    FrameId = JointTrajectoryControllerStatePublisherParameters->FrameId;
    JointParamTopic = JointTrajectoryControllerStatePublisherParameters->JointParamTopic;
    JointTrajectoryControllerName = JointTrajectoryControllerStatePublisherParameters->JointTrajectoryControllerName;
  }
}

void URJointTrajectoryControllerStatePublisher::Init()
{
  Super::Init();
  if (GetOwner())
  {
    JointTrajectoryController = Cast<URJointTrajectoryController>(GetOwner()->GetController(JointTrajectoryControllerName));
    if (JointTrajectoryController)
    {
      UE_LOG(LogTemp, Error, TEXT("Configure JointControllerStatePublisher publisher"));
      ConfigClient = NewObject<URJointStateConfigurationClient>(GetOwner());
      ConfigClient->JointParamTopic = JointParamTopic;
      ConfigClient->Connect(Handler);
      ConfigClient->GetJointNames([this](const TArray<FString> &JointNames){JointList = JointNames;});
    }
  }
}

void URJointTrajectoryControllerStatePublisher::Publish()
{
  if (GetOwner() && JointTrajectoryController)
  {
    static int Seq = 0;

    TSharedPtr<control_msgs::JointTrajectoryControllerState> State =
        MakeShareable(new control_msgs::JointTrajectoryControllerState());

    State->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

    TArray<FString> JointNames;
    TArray<double> DesiredPositions;
    TArray<double> CurrentPositions;
    TArray<double> ErrorPositions;
    TArray<double> DesiredVelocities;
    TArray<double> CurrentVelocities;
    TArray<double> ErrorVelocities;
    // for (const TPair<FString, FJointState> &DesiredJointState : JointTrajectoryController->DesiredJointStates)
    for (const FString  &JointName : JointList)
    {
      if(JointTrajectoryController->DesiredJointStates.Contains(JointName))
        {
          FJointState &DesiredJointState = JointTrajectoryController->DesiredJointStates[JointName];
          if (URJoint *Joint = GetOwner()->GetJoint(JointName))
            {
              JointNames.Add(JointName);

              DesiredPositions.Add(DesiredJointState.JointPosition);
              DesiredVelocities.Add(DesiredJointState.JointVelocity);

              CurrentPositions.Add(Joint->GetJointState().JointPosition);
              CurrentVelocities.Add(Joint->GetJointState().JointVelocity);

              ErrorPositions.Add(DesiredJointState.JointPosition - Joint->GetJointState().JointPosition);
              ErrorVelocities.Add(DesiredJointState.JointVelocity - Joint->GetJointState().JointVelocity);
            }
        }
    }

    State->SetJointNames(JointNames);

    trajectory_msgs::JointTrajectoryPoint DesiredMsg;
    DesiredMsg.SetPositions(DesiredPositions);
    DesiredMsg.SetVelocities(DesiredVelocities);
    State->SetDesired(DesiredMsg);

    trajectory_msgs::JointTrajectoryPoint ActualMsg;
    ActualMsg.SetPositions(CurrentPositions);
    ActualMsg.SetVelocities(CurrentVelocities);
    State->SetActual(ActualMsg);

    trajectory_msgs::JointTrajectoryPoint ErrorMsg;
    ErrorMsg.SetPositions(ErrorPositions);
    ErrorMsg.SetVelocities(ErrorVelocities);
    State->SetError(ErrorMsg);

    // State->SetJointNames(JointController->TrajectoryStatus.JointNames);


    Handler->PublishMsg(Topic, State);
    Handler->Process();

    Seq++;
  }
}
