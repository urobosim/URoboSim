
#include "ROSCommunication/Actions/PR2HeadAction/PR2HAResultPublisher.h"
#include "pr2_controllers_msgs/PR2PointHeadActionResult.h"
#include "pr2_controllers_msgs/PR2PointHeadResult.h"

void URPR2HeadActionResultPublisher::SetMessageType()
{
  MessageType = TEXT("pr2_controllers_msgs/PointHeadActionResult");
}

void URPR2HeadActionResultPublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URHeadTrajectoryController>(ControllerComp->Controller.ControllerList["HeadController"]);
}

void URPR2HeadActionResultPublisher::Publish()
{
  if(Owner->bPublishResult)
    {
      UE_LOG(LogTemp, Error, TEXT("Publish Result HeadAction"));
      TSharedPtr<pr2_controllers_msgs::PR2PointHeadActionResult> ActionResult =
        MakeShareable(new pr2_controllers_msgs::PR2PointHeadActionResult());

      ActionResult->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

      // uint8 status = Owner->Status;
      FGoalStatusInfo StatusInfo = Owner->GoalStatusList.Last();
      actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, "");
      ActionResult->SetStatus(GS);

      pr2_controllers_msgs::PR2PointHeadResult Result;
      // control_msgs::PointHeadResult Result();
      ActionResult->SetResult(Result);

      Handler->PublishMsg(Topic, ActionResult);
      Handler->Process();

      Seq++ ;
      Owner->bPublishResult = false;
    }
}
