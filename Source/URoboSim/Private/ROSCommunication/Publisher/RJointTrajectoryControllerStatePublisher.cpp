#include "ROSCommunication/Publisher/RJointTrajectoryControllerStatePublisher.h"
#include "Controller/RControllerComponent.h"
#include "control_msgs/JointTrajectoryControllerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJointTrajectoryControllerStatePublisher, Log, All);

URJointTrajectoryControllerStatePublisher::URJointTrajectoryControllerStatePublisher()
{
  Topic = TEXT("/whole_body_controller/body/state");
  MessageType = TEXT("control_msgs/JointTrajectoryControllerState");
}

void URJointTrajectoryControllerStatePublisher::Init()
{
  if (GetOwner())
  {
    URControllerComponent *ControllerComponent = Cast<URControllerComponent>(GetOwner()->GetPlugin(TEXT("ControllerComponent")));
    if (ControllerComponent)
    {
      JointController = Cast<URJointController>(ControllerComponent->GetController(TEXT("JointController")));
    }
    else
    {
      UE_LOG(LogRJointTrajectoryControllerStatePublisher, Error, TEXT("ControllerComponent not found in %s"), *GetName())
    }
  }
}

void URJointTrajectoryControllerStatePublisher::Publish()
{
  if (JointController)
  {
    static int Seq = 0;

    TSharedPtr<control_msgs::JointTrajectoryControllerState> State =
    MakeShareable(new control_msgs::JointTrajectoryControllerState());

    State->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

    TArray<FString> JointNames;
    TArray<double> DesiredPositions;
    TArray<double> CurrentPositions;
    TArray<double> ErrorPositions;
    for (const FTrajectoryStatus &TrajectoryStatus : JointController->GetTrajectoryStatusArray())
    {
      JointNames.Add(TrajectoryStatus.JointName);
      DesiredPositions.Add(TrajectoryStatus.DesiredPosition);
      CurrentPositions.Add(TrajectoryStatus.CurrentPosition);
      ErrorPositions.Add(TrajectoryStatus.ErrorPosition);
    }

    State->SetJointNames(JointNames);

    trajectory_msgs::JointTrajectoryPoint DesiredPositionsMsg;
    DesiredPositionsMsg.SetPositions(DesiredPositions);
    State->SetDesired(DesiredPositionsMsg);

    trajectory_msgs::JointTrajectoryPoint CurrentPositionMsg;
    CurrentPositionMsg.SetPositions(CurrentPositions);
    State->SetActual(CurrentPositionMsg);

    trajectory_msgs::JointTrajectoryPoint ErrorPositionMsg;
    ErrorPositionMsg.SetPositions(ErrorPositions);
    State->SetError(ErrorPositionMsg);

    Handler->PublishMsg(Topic, State);
    Handler->Process();

    Seq++;
  }
}
