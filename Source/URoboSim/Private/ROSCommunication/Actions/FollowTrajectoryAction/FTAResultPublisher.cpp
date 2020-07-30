#include "ROSCommunication/Actions/FollowTrajectoryAction/FTAResultPublisher.h"
#include "control_msgs/FollowJointTrajectoryActionResult.h"
#include "control_msgs/FollowJointTrajectoryResult.h"

void URFollowTrajectoryActionResultPublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URJointController>(ControllerComp->Controller.ControllerList["JointController"]);
  /* Controller->ControllerList["JointController"]; */
}

void URFollowTrajectoryActionResultPublisher::SetMessageType()
{
  MessageType = TEXT("control_msgs/FollowJointTrajectoryActionResult");
}

void URFollowTrajectoryActionResultPublisher::Publish()
{
  /* UE_LOG(LogTemp, Error, TEXT("bPublishResult pub: %s"), Owner->bPublishResult ? TEXT("True") : TEXT("False")); */
  if(Owner->bPublishResult)
    {
      UE_LOG(LogTemp, Error, TEXT("Publish Result FollowJointTrajectory"));
      TSharedPtr<control_msgs::FollowJointTrajectoryActionResult> ActionResult =
        MakeShareable(new control_msgs::FollowJointTrajectoryActionResult());

      ActionResult->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

      // uint8 status = Owner->Status;
      FGoalStatusInfo StatusInfo = Owner->GoalStatusList.Last();
      actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, "");
      ActionResult->SetStatus(GS);

      control_msgs::FollowJointTrajectoryResult Result(0);
      ActionResult->SetResult(Result);

      Handler->PublishMsg(Topic, ActionResult);
      Handler->Process();

      Seq++ ;
      Owner->bPublishResult = false;
    }
}
