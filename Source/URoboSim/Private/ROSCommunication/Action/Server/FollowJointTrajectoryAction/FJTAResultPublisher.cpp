#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAResultPublisher.h"
#include "Controller/RControllerComponent.h"
#include "control_msgs/FollowJointTrajectoryActionResult.h"
#include "control_msgs/FollowJointTrajectoryResult.h"

DEFINE_LOG_CATEGORY_STATIC(LogRFJTAResultPublisher, Log, All);

URFJTAResultPublisher::URFJTAResultPublisher()
{
  PublisherParameters = CreateDefaultSubobject<URFJTAResultPublisherParameter>(TEXT("FJTAResultPublisherParameters"));
}

void URFJTAResultPublisher::Init()
{
  if (!PublisherParameters)
  {
    PublisherParameters = CreateDefaultSubobject<URFJTAResultPublisherParameter>(TEXT("FJTAResultPublisherParameters"));
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
      UE_LOG(LogRFJTAResultPublisher, Error, TEXT("JointController not found in %s"), *GetName())
    }
  }
}

void URFJTAResultPublisher::Publish()
{
  if (JointController->bPublishResult)
  {
    static int Seq = 0;
    const URFJTAResultPublisherParameter *FJTAResultPublisherParameters = GetFJTAResultPublisherParameters();
    if (FJTAResultPublisherParameters)
    {
      UE_LOG(LogRFJTAResultPublisher, Log, TEXT("The goal of %s has been reached, publishing results"), *GetOwner()->GetName());
      TSharedPtr<control_msgs::FollowJointTrajectoryActionResult> ActionResult =
          MakeShareable(new control_msgs::FollowJointTrajectoryActionResult());

      ActionResult->SetHeader(std_msgs::Header(Seq, FROSTime(), FJTAResultPublisherParameters->FrameId));

      FGoalStatusInfo GoalStatusInfo = JointController->GetGoalStatus();
      actionlib_msgs::GoalStatus GoalStatus(actionlib_msgs::GoalID(FROSTime(GoalStatusInfo.Secs, GoalStatusInfo.NSecs), GoalStatusInfo.Id), GoalStatusInfo.Status, TEXT(""));
      ActionResult->SetStatus(GoalStatus);

      control_msgs::FollowJointTrajectoryResult Result(0);
      ActionResult->SetResult(Result);

      Handler->PublishMsg(FJTAResultPublisherParameters->Topic, ActionResult);
      Handler->Process();

      JointController->bPublishResult = false;
      Seq++;
    }
  }
}
