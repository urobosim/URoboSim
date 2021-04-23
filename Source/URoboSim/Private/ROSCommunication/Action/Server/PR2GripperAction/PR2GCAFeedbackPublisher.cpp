#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GCAFeedbackPublisher.h"
#include "pr2_controllers_msgs/PR2GripperCommandActionFeedback.h"

void URPR2GripperCommandActionFeedbackPublisher::SetMessageType()
{
  MessageType = TEXT("pr2_controllers_msgs/Pr2GripperCommandActionFeedback");
}

void URPR2GripperCommandActionFeedbackPublisher::SetOwner(UObject* InOwner)
{
  ARModel* Model =Cast<ARModel>(InOwner);
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Model->Plugins["ControllerComponent"]);
  Owner = Cast<URGripperController>(ControllerComp->Controller.ControllerList[ControllerName]);
}

void URPR2GripperCommandActionFeedbackPublisher::Publish()
{
  if(Owner->bActive)
    {
      TSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionFeedback> Feedback =
        MakeShareable(new pr2_controllers_msgs::PR2GripperCommandActionFeedback());

      FGoalStatusInfo StatusInfo = Owner->GoalStatusList.Last();
      actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, "");
      Feedback->SetStatus(GS);
      Feedback->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

      pr2_controllers_msgs::PR2GripperCommandFeedback FeedbackMsg(Owner->Result.Position / 100.0, Owner->Result.Effort , Owner->Result.bStalled , Owner->Result.bReachedGoal );

      Feedback->SetFeedback(FeedbackMsg);
      Handler->PublishMsg(Topic, Feedback);
      Handler->Process();

      // UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
      Seq += 1;
    }
}
