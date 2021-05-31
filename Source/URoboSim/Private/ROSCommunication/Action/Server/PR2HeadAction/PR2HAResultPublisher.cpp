#include "ROSCommunication/Action/Server/PR2HeadAction/PR2HAResultPublisher.h"
#include "pr2_controllers_msgs/PR2PointHeadActionResult.h"
#include "pr2_controllers_msgs/PR2PointHeadResult.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPR2HAResultPublisher, Log, All)

URPR2HAResultPublisher::URPR2HAResultPublisher()
{
  MessageType = TEXT("pr2_controllers_msgs/PointHeadActionResult");
}

void URPR2HAResultPublisher::Init()
{
  Super::Init();
  HeadController = Cast<URHeadTrajectoryController>(Controller);
}

void URPR2HAResultPublisher::Publish()
{
  if (HeadController->bPublishResult)
  {
    static int Seq = 0;
    UE_LOG(LogRPR2HAResultPublisher, Error, TEXT("Publish Result HeadAction"));
    TSharedPtr<pr2_controllers_msgs::PR2PointHeadActionResult> ActionResult =
        MakeShareable(new pr2_controllers_msgs::PR2PointHeadActionResult());

    ActionResult->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

    // uint8 status = Owner->Status;
    FGoalStatusInfo StatusInfo = HeadController->GetGoalStatusList().Last();
    actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, TEXT(""));
    ActionResult->SetStatus(GS);

    pr2_controllers_msgs::PR2PointHeadResult Result;
    // control_msgs::PointHeadResult Result();
    ActionResult->SetResult(Result);

    Handler->PublishMsg(Topic, ActionResult);
    Handler->Process();

    Seq++;
    HeadController->bPublishResult = false;
  }
}
