#include "ROSCommunication/Actions/PR2GripperAction/PR2GCAResultPublisher.h"
#include "pr2_controllers_msgs/PR2GripperCommandActionResult.h"
#include "pr2_controllers_msgs/PR2GripperCommandResult.h"

void URPR2GripperCommandActionResultPublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URGripperController>(ControllerComp->Controller.ControllerList[ControllerName]);
}

void URPR2GripperCommandActionResultPublisher::SetMessageType()
{
  MessageType = TEXT("pr2_controllers_msgs/Pr2GripperCommandActionResult");
}

void URPR2GripperCommandActionResultPublisher::Publish()
{
  if(Owner->bPublishResult)
    {
      TSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionResult> ActionResult =
        MakeShareable(new pr2_controllers_msgs::PR2GripperCommandActionResult());

      ActionResult->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

      // uint8 status = Owner->Status;
      FGoalStatusInfo StatusInfo = Owner->GoalStatusList.Last();
      actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, "");
      ActionResult->SetStatus(GS);

      //TODO make use of real values
      // pr2_controllers_msgs::PR2GripperCommandResult Result(Owner->Result.Position, Owner->Result.Effort , Owner->Result.bStalled , Owner->Result.bReachedGoal );
      pr2_controllers_msgs::PR2GripperCommandResult Result(Owner->Result.Position / 100.0, Owner->Result.Effort , Owner->Result.bStalled , Owner->Result.bReachedGoal );
      ActionResult->SetResult(Result);

      Handler->PublishMsg(Topic, ActionResult);
      Handler->Process();

      Seq++ ;
      Owner->bPublishResult = false;
      UE_LOG(LogTemp, Error, TEXT("Publish Result Gripper Action"));
    }
}
