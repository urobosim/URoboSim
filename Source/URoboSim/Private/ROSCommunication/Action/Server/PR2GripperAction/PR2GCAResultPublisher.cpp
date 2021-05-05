#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GCAResultPublisher.h"
#include "pr2_controllers_msgs/PR2GripperCommandActionResult.h"
#include "pr2_controllers_msgs/PR2GripperCommandResult.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPR2GCAResultPublisher, Log, All);

URPR2GCAResultPublisher::URPR2GCAResultPublisher()
{
  MessageType = TEXT("pr2_controllers_msgs/Pr2GripperCommandActionResult");
}

void URPR2GCAResultPublisher::Init()
{
  Super::Init();
  GripperController = Cast<URGripperController>(Controller);
}

void URPR2GCAResultPublisher::Publish()
{
  if (GetOwner() && GripperController->bPublishResult)
  {
    static int Seq = 0;
    TSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionResult> ActionResult =
        MakeShareable(new pr2_controllers_msgs::PR2GripperCommandActionResult());

    ActionResult->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

    // uint8 status = Owner->Status;
    FGoalStatusInfo StatusInfo = GripperController->GetGoalStatusList().Last();
    actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, "");
    ActionResult->SetStatus(GS);

    //TODO make use of real values
    // pr2_controllers_msgs::PR2GripperCommandResult Result(Owner->Result.Position, Owner->Result.Effort , Owner->Result.bStalled , Owner->Result.bReachedGoal );
    pr2_controllers_msgs::PR2GripperCommandResult Result(GripperController->Result.Position / 100.0, GripperController->Result.Effort, GripperController->Result.bStalled, GripperController->Result.bReachedGoal);
    ActionResult->SetResult(Result);

    Handler->PublishMsg(Topic, ActionResult);
    Handler->Process();

    Seq++;
    GripperController->bPublishResult = false;
    UE_LOG(LogRPR2GCAResultPublisher, Error, TEXT("Publish Result Gripper Action"));
  }
}
