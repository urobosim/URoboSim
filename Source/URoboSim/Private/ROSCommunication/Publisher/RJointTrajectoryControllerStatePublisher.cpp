#include "ROSCommunication/Publisher/RJointTrajectoryControllerStatePublisher.h"
#include "Controller/RControllerComponent.h"
#include "control_msgs/JointTrajectoryControllerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJointTrajectoryControllerStatePublisher, Log, All);

URJointTrajectoryControllerStatePublisher::URJointTrajectoryControllerStatePublisher()
{
  Topic = TEXT("/whole_body_controller/body/state");
  MessageType = TEXT("control_msgs/JointTrajectoryControllerState");
  FrameId = TEXT("odom");
  JointControllerName = TEXT("JointController");
}

void URJointTrajectoryControllerStatePublisher::Init()
{
  if (GetOwner())
  {
    JointController = Cast<URJointController>(GetOwner()->GetController(JointControllerName));
  }
}

void URJointTrajectoryControllerStatePublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  URJointTrajectoryControllerStatePublisherParameter *JointTrajectoryControllerStatePublisherParameters = Cast<URJointTrajectoryControllerStatePublisherParameter>(PublisherParameters);
  if (JointTrajectoryControllerStatePublisherParameters)
  {
    Super::SetPublishParameters(PublisherParameters);
    FrameId = JointTrajectoryControllerStatePublisherParameters->FrameId;
    JointControllerName = JointTrajectoryControllerStatePublisherParameters->JointControllerName;
  }
}

void URJointTrajectoryControllerStatePublisher::Publish()
{
  if (JointController)
  {
    static int Seq = 0;
    static TSharedPtr<control_msgs::JointTrajectoryControllerState> State =
        MakeShareable(new control_msgs::JointTrajectoryControllerState());

    State->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

    TArray<FString> JointNames;
    TArray<double> DesiredPositions;
    TArray<double> CurrentPositions;
    TArray<double> ErrorPositions;
    TArray<double> DesiredVelocities;
    TArray<double> CurrentVelocities;
    TArray<double> ErrorVelocities;
    if (JointController->GetTrajectoryStatusArray().Num() > 0)
    {
      for (const FTrajectoryStatus &TrajectoryStatus : JointController->GetTrajectoryStatusArray())
      {
        JointNames.Add(TrajectoryStatus.JointName);
        DesiredPositions.Add(TrajectoryStatus.DesiredState.JointPosition);
        CurrentPositions.Add(TrajectoryStatus.CurrentState.JointPosition);
        ErrorPositions.Add(TrajectoryStatus.ErrorState.JointPosition);
        DesiredVelocities.Add(TrajectoryStatus.DesiredState.JointVelocity);
        CurrentVelocities.Add(TrajectoryStatus.CurrentState.JointVelocity);
        ErrorVelocities.Add(TrajectoryStatus.ErrorState.JointVelocity);
      }
    }

    State->SetJointNames(JointNames);

    trajectory_msgs::JointTrajectoryPoint DesiredStatesMsg;
    DesiredStatesMsg.SetPositions(DesiredPositions);
    DesiredStatesMsg.SetVelocities(DesiredVelocities);
    State->SetDesired(DesiredStatesMsg);

    trajectory_msgs::JointTrajectoryPoint CurrentStatesMsg;
    CurrentStatesMsg.SetPositions(CurrentPositions);
    CurrentStatesMsg.SetVelocities(CurrentVelocities);
    State->SetActual(CurrentStatesMsg);

    trajectory_msgs::JointTrajectoryPoint ErrorStatesMsg;
    ErrorStatesMsg.SetPositions(ErrorPositions);
    ErrorStatesMsg.SetVelocities(ErrorVelocities);
    State->SetError(ErrorStatesMsg);

    Handler->PublishMsg(Topic, State);
    Handler->Process();

    Seq++;
  }
  else
  {
    UE_LOG(LogRJointTrajectoryControllerStatePublisher, Error, TEXT("%s not found in %s"), *JointControllerName, *GetName())
  }
}
