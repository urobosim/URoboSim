#include "ROSCommunication/Actions/FollowTrajectoryAction/FTAResultPublisher.h"
#include "control_msgs/FollowJointTrajectoryActionResult.h"
#include "control_msgs/FollowJointTrajectoryResult.h"

void URFollowTrajectoryActionResultPublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URJointController>(ControllerComp->Controller.ControllerList["JointController"]);
  Owner->ActionFinished.AddDynamic(this, &URFollowTrajectoryActionResultPublisher::PublishResult);
  /* Controller->ControllerList["JointController"]; */
}

void URFollowTrajectoryActionResultPublisher::SetMessageType()
{
  MessageType = TEXT("control_msgs/FollowJointTrajectoryActionResult");
}

void URFollowTrajectoryActionResultPublisher::Publish()
{

}

void URFollowTrajectoryActionResultPublisher::PublishResult(FGoalStatusInfo InStatusInfo)
{
  // if(Owner->bPublishResult)

  UE_LOG(LogTemp, Error, TEXT("Publish Result FollowJointTrajectory"));
  TSharedPtr<control_msgs::FollowJointTrajectoryActionResult> ActionResult =
    MakeShareable(new control_msgs::FollowJointTrajectoryActionResult());

  ActionResult->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

  // uint8 status = Owner->Status;
  actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(InStatusInfo.Secs, InStatusInfo.NSecs), InStatusInfo.Id), InStatusInfo.Status, "");
  ActionResult->SetStatus(GS);

  control_msgs::FollowJointTrajectoryResult Result(0);
  ActionResult->SetResult(Result);

  Handler->PublishMsg(Topic, ActionResult);
  Handler->Process();

  Seq++ ;

  // Owner->GoalStatusList.RemoveSingle(Status);
  UE_LOG(LogTemp, Error, TEXT("Publish finished"));



  // while(IndexArray.Num() != 0)
  //   {
  //     Index = IndexArray.Pop();
  //     Owner->GoalStatusList.RemoveAt(Index);
  //   }
  // for(auto& i : IndexArray)
  //   {
  //     Owner->GoalStatusList.RemoveAt(i);
  //   }
}
