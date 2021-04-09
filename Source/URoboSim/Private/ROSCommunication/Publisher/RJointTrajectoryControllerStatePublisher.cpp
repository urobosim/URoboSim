#include "ROSCommunication/Publisher/RJointTrajectoryControllerStatePublisher.h"
#include "Controller/RControllerComponent.h"
#include "control_msgs/JointTrajectoryControllerState.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJointTrajectoryControllerStatePublisher, Log, All);

void URJointTrajectoryControllerStatePublisher::Init()
{
  if (!PublisherParameters)
  {
    PublisherParameters = CreateDefaultSubobject<URJointTrajectoryControllerStatePublisherParameter>(TEXT("JointStatePublisherParameters"));
  }

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
    const URJointTrajectoryControllerStatePublisherParameter *JointTrajectoryControllerStatePublisherParameters = GetJointTrajectoryControllerStatePublisherParameters();
    if (JointTrajectoryControllerStatePublisherParameters)
    {
      static TSharedPtr<control_msgs::JointTrajectoryControllerState> State =
          MakeShareable(new control_msgs::JointTrajectoryControllerState());

      State->SetHeader(std_msgs::Header(Seq, FROSTime(), JointTrajectoryControllerStatePublisherParameters->FrameId));

      TArray<FString> JointNames;
      TArray<double> DesiredPositions;
      TArray<double> CurrentPositions;
      TArray<double> ErrorPositions;
      TArray<double> DesiredVelocities;
      TArray<double> CurrentVelocities;
      TArray<double> ErrorVelocities;
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

      Handler->PublishMsg(JointTrajectoryControllerStatePublisherParameters->Topic, State);
      Handler->Process();

      Seq++;
    }
  }
  else
  {
    UE_LOG(LogRJointTrajectoryControllerStatePublisher, Error, TEXT("JointController not found in %s"), *GetName())
  }
}
