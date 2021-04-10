#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAFeedbackPublisher.h"
#include "Controller/RControllerComponent.h"
#include "control_msgs/FollowJointTrajectoryActionFeedback.h"
#include "trajectory_msgs/JointTrajectoryPoint.h"

DEFINE_LOG_CATEGORY_STATIC(LogRFJTAFeedbackPublisher, Log, All)

URFJTAFeedbackPublisher::URFJTAFeedbackPublisher()
{
  MessageType = TEXT("control_msgs/FollowJointTrajectoryActionFeedback");
  FrameId = TEXT("odom");
  JointParamPath = TEXT("whole_body_controller/body/joints");
  JointControllerName = TEXT("JointController");
}

void URFJTAFeedbackPublisher::Init()
{
  if (GetOwner())
  {
    URControllerComponent *ControllerComponent = Cast<URControllerComponent>(GetOwner()->GetPlugin(TEXT("ControllerComponent")));
    if (ControllerComponent)
    {
      JointController = Cast<URJointController>(ControllerComponent->GetController(JointControllerName));
    }
    else
    {
      UE_LOG(LogRFJTAFeedbackPublisher, Error, TEXT("ControllerComponent not found in %s"), *GetName())
    }

    GetJointsClient = NewObject<URGetJointsClient>();
    GetJointsClient->GetParamArgument.Name = JointParamPath;
    GetJointsClient->URServiceClient::Init(GetOwner(), Handler);
  }
}

void URFJTAFeedbackPublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  URFJTAFeedbackPublisherParameter *FJTAFeedbackPublisherParameters = Cast<URFJTAFeedbackPublisherParameter>(PublisherParameters);
  if (FJTAFeedbackPublisherParameters)
  {
    Super::SetPublishParameters(PublisherParameters);
    FrameId = FJTAFeedbackPublisherParameters->FrameId;
    JointParamPath = FJTAFeedbackPublisherParameters->JointParamPath;
  }  
}

void URFJTAFeedbackPublisher::Publish()
{
  if (JointController)
  {
    if (JointController->State == UJointControllerState::FollowJointTrajectory)
    {
      static int Seq = 0;
      TSharedPtr<control_msgs::FollowJointTrajectoryActionFeedback> Feedback =
          MakeShareable(new control_msgs::FollowJointTrajectoryActionFeedback());

      Feedback->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

      FGoalStatusInfo GoalStatusInfo = JointController->GetGoalStatus();
      actionlib_msgs::GoalStatus GoalStatus(actionlib_msgs::GoalID(FROSTime(GoalStatusInfo.Secs, GoalStatusInfo.NSecs), GoalStatusInfo.Id), GoalStatusInfo.Status, TEXT(""));
      Feedback->SetStatus(GoalStatus);

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

      Seq++;
    }
  }
  else
  {
    UE_LOG(LogRFJTAFeedbackPublisher, Error, TEXT("JointController not found in %s"), *GetName())
  }
}