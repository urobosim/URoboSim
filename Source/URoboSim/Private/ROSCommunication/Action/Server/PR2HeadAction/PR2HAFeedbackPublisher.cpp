#include "ROSCommunication/Actions/PR2HeadAction/PR2HAFeedbackPublisher.h"
#include "pr2_controllers_msgs/PR2PointHeadActionFeedback.h"

void URPR2HeadActionFeedbackPublisher::SetMessageType()
{
  MessageType = TEXT("pr2_controllers_msgs/PointHeadActionFeedback");
}

void URPR2HeadActionFeedbackPublisher::SetOwner(UObject* InOwner)
{
  // Owner = Cast<ARModel>(InOwner);
  ARModel* Model =Cast<ARModel>(InOwner);
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Model->Plugins["ControllerComponent"]);
  Owner = Cast<URHeadTrajectoryController>(ControllerComp->Controller.ControllerList["HeadController"]);
}

void URPR2HeadActionFeedbackPublisher::Publish()
{
  if(Owner->bActive)
    {
      TSharedPtr<pr2_controllers_msgs::PR2PointHeadActionFeedback> Feedback =
        MakeShareable(new pr2_controllers_msgs::PR2PointHeadActionFeedback());

      FGoalStatusInfo StatusInfo = Owner->GoalStatusList.Last();
      actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, "");
      Feedback->SetStatus(GS);
      Feedback->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

      pr2_controllers_msgs::PR2PointHeadFeedback FeedbackMsg;

      FeedbackMsg.SetPointingAngleError(0);
      Feedback->SetFeedback(FeedbackMsg);
      Handler->PublishMsg(Topic, Feedback);
      Handler->Process();

      // UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
      Seq += 1;
    }
}
