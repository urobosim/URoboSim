#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAResultPublisher.h"
#include "control_msgs/FollowJointTrajectoryActionResult.h"
#include "control_msgs/FollowJointTrajectoryResult.h"

DEFINE_LOG_CATEGORY_STATIC(LogRFJTAResultPublisher, Log, All);

URFJTAResultPublisher::URFJTAResultPublisher()
{
  MessageType = TEXT("control_msgs/FollowJointTrajectoryActionResult");
  FrameId = TEXT("odom");
}

void URFJTAResultPublisher::Init()
{
  JointController = Cast<URJointController>(Controller);
}

void URFJTAResultPublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  URFJTAResultPublisherParameter *FJTAResultPublisherParameters = Cast<URFJTAResultPublisherParameter>(PublisherParameters);
  if (FJTAResultPublisherParameters)
  {
    Super::SetPublishParameters(PublisherParameters);
    FrameId = FJTAResultPublisherParameters->FrameId;
  }  
}

void URFJTAResultPublisher::Publish()
{
  if (JointController && JointController->bPublishResult)
  {
    static int Seq = 0;

    UE_LOG(LogRFJTAResultPublisher, Log, TEXT("The goal of %s has been reached, publishing results"), *GetOwner()->GetName());
    TSharedPtr<control_msgs::FollowJointTrajectoryActionResult> ActionResult =
        MakeShareable(new control_msgs::FollowJointTrajectoryActionResult());

    ActionResult->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

    FGoalStatusInfo GoalStatusInfo = JointController->GetGoalStatus();
    actionlib_msgs::GoalStatus GoalStatus(actionlib_msgs::GoalID(FROSTime(GoalStatusInfo.Secs, GoalStatusInfo.NSecs), GoalStatusInfo.Id), GoalStatusInfo.Status, TEXT(""));
    ActionResult->SetStatus(GoalStatus);

    control_msgs::FollowJointTrajectoryResult Result(0);
    ActionResult->SetResult(Result);

    Handler->PublishMsg(Topic, ActionResult);
    Handler->Process();

    JointController->bPublishResult = false;
    Seq++;
  }
}
