#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAFeedbackPublisher.h"
#include "Controller/RControllerComponent.h"
#include "control_msgs/FollowJointTrajectoryActionFeedback.h"
#include "trajectory_msgs/JointTrajectoryPoint.h"

DEFINE_LOG_CATEGORY_STATIC(LogRFJTAFeedbackPublisher, Log, All)

void URFJTAFeedbackPublisher::Init()
{
  if (!PublisherParameters)
  {
    PublisherParameters = CreateDefaultSubobject<URFJTAFeedbackPublisherParameter>(TEXT("FJTAFeedbackPublisherParameters"));
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
      UE_LOG(LogRFJTAFeedbackPublisher, Error, TEXT("ControllerComponent not found in %s"), *GetName())
    }
    
    const URFJTAFeedbackPublisherParameter *FJTAFeedbackPublisherParameters = GetFJTAFeedbackPublisherParameters();
    if (FJTAFeedbackPublisherParameters)
    {
      GetJointsClient = NewObject<URGetJointsClient>();
      GetJointsClient->GetParamArgument.Name = FJTAFeedbackPublisherParameters->JointParamPath;
      GetJointsClient->URServiceClient::Init(GetOwner(), Handler);
    }
  }
}

void URFJTAFeedbackPublisher::Publish()
{
  if (JointController)
  {
    if (JointController->State == UJointControllerState::FollowJointTrajectory)
    {
      static int Seq = 0;
      const URFJTAFeedbackPublisherParameter *FJTAFeedbackPublisherParameters = GetFJTAFeedbackPublisherParameters();
      if (FJTAFeedbackPublisherParameters)
      {
        TSharedPtr<control_msgs::FollowJointTrajectoryActionFeedback> Feedback =
            MakeShareable(new control_msgs::FollowJointTrajectoryActionFeedback());

        Feedback->SetHeader(std_msgs::Header(Seq, FROSTime(), FJTAFeedbackPublisherParameters->FrameId));

        FGoalStatusInfo GoalStatusInfo = JointController->GetGoalStatus();
        actionlib_msgs::GoalStatus GoalStatus(actionlib_msgs::GoalID(FROSTime(GoalStatusInfo.Secs, GoalStatusInfo.NSecs), GoalStatusInfo.Id), GoalStatusInfo.Status, TEXT(""));
        Feedback->SetStatus(GoalStatus);

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

        control_msgs::FollowJointTrajectoryFeedback FeedbackMsg;
        FeedbackMsg.SetHeader(std_msgs::Header(Seq, FROSTime(), FJTAFeedbackPublisherParameters->FrameId));
        FeedbackMsg.SetJointNames(JointNames);

        trajectory_msgs::JointTrajectoryPoint DesiredPositionsMsg;
        DesiredPositionsMsg.SetPositions(DesiredPositions);
        FeedbackMsg.SetDesired(DesiredPositionsMsg);

        trajectory_msgs::JointTrajectoryPoint CurrentPositionsMsg;
        CurrentPositionsMsg.SetPositions(CurrentPositions);
        FeedbackMsg.SetActual(CurrentPositionsMsg);

        trajectory_msgs::JointTrajectoryPoint ErrorPositionsMsg;
        ErrorPositionsMsg.SetPositions(ErrorPositions);
        FeedbackMsg.SetError(ErrorPositionsMsg);
        Feedback->SetFeedback(FeedbackMsg);

        Handler->PublishMsg(FJTAFeedbackPublisherParameters->Topic, Feedback);
        Handler->Process();

        Seq++;
      }
    }
  }
  else
  {
    UE_LOG(LogRFJTAFeedbackPublisher, Error, TEXT("JointController not found in %s"), *GetName())
  }
}