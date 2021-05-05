#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GCAFeedbackPublisher.h"
#include "pr2_controllers_msgs/PR2GripperCommandActionFeedback.h"

URPR2GCAFeedbackPublisher::URPR2GCAFeedbackPublisher()
{
  MessageType = TEXT("pr2_controllers_msgs/Pr2GripperCommandActionFeedback");
}

void URPR2GCAFeedbackPublisher::Init()
{
  Super::Init();
  GripperController = Cast<URGripperController>(Controller);
}

void URPR2GCAFeedbackPublisher::Publish()
{
  if (GripperController->bActive)
  {
    static int Seq = 0;

    TSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionFeedback> Feedback =
        MakeShareable(new pr2_controllers_msgs::PR2GripperCommandActionFeedback());

    FGoalStatusInfo StatusInfo = GripperController->GetGoalStatusList().Last();
    actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, "");
    Feedback->SetStatus(GS);
    Feedback->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

    pr2_controllers_msgs::PR2GripperCommandFeedback FeedbackMsg(GripperController->Result.Position / 100.0, GripperController->Result.Effort, GripperController->Result.bStalled, GripperController->Result.bReachedGoal);

    Feedback->SetFeedback(FeedbackMsg);
    Handler->PublishMsg(Topic, Feedback);
    Handler->Process();

    // UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
    Seq++;
  }
}
