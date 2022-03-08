#include "ROSCommunication/Action/Server/PR2HeadAction/PR2HAFeedbackPublisher.h"
#include "pr2_controllers_msgs/PR2PointHeadActionFeedback.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPRHAFeedbackPublisher, Log, All)

URPR2HAFeedbackPublisher::URPR2HAFeedbackPublisher()
{
  MessageType = TEXT("pr2_controllers_msgs/PointHeadActionFeedback");
}

void URPR2HAFeedbackPublisher::Init()
{
  Super::Init();
  HeadController = Cast<URHeadTrajectoryController>(Controller);
}

void URPR2HAFeedbackPublisher::Publish()
{
  if (HeadController->bActive)
  {
    static int Seq = 0;

    TSharedPtr<pr2_controllers_msgs::PR2PointHeadActionFeedback> Feedback =
        MakeShareable(new pr2_controllers_msgs::PR2PointHeadActionFeedback());

    FGoalStatusInfo StatusInfo = HeadController->GetGoalStatusList().Last();
    actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, TEXT(""));
    Feedback->SetStatus(GS);
    Feedback->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

    pr2_controllers_msgs::PR2PointHeadFeedback FeedbackMsg;

    FeedbackMsg.SetPointingAngleError(0);
    Feedback->SetFeedback(FeedbackMsg);
    Handler->PublishMsg(Topic, Feedback);
    Handler->Process();

    // UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
    Seq += 1;
  }
}
