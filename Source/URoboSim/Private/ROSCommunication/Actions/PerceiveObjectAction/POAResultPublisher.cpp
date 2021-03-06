
#include "ROSCommunication/Actions/PerceiveObjectAction/POAResultPublisher.h"
#include "urobosim_msgs/PerceiveObjectActionResult.h"
#include "urobosim_msgs/PerceiveObjectResult.h"

void URPerceiveObjectActionResultPublisher::SetMessageType()
{
  MessageType = TEXT("urobosim_msgs/PerceiveObjectActionResult");
}

void URPerceiveObjectActionResultPublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URCameraController>(ControllerComp->Controller.ControllerList["CameraController"]);
}

void URPerceiveObjectActionResultPublisher::Publish()
{
  if(Owner->bPublishResult)
    {
      TSharedPtr<urobosim_msgs::PerceiveObjectActionResult> ActionResult =
        MakeShareable(new urobosim_msgs::PerceiveObjectActionResult());

      ActionResult->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

      // uint8 status = Owner->Status;
      FGoalStatusInfo StatusInfo = Owner->GoalStatusList.Last();
      actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, "");
      ActionResult->SetStatus(GS);

      // In Base Footprint
      geometry_msgs::Point Point(FConversions::UToROS(Owner->PerceivedObject->Pose.GetLocation()));
      geometry_msgs::Quaternion Orientation(FConversions::UToROS(Owner->PerceivedObject->Pose.GetRotation()));

      geometry_msgs::Pose Pose(Point, Orientation);
      geometry_msgs::PoseStamped PoseStamped(std_msgs::Header(Seq, FROSTime(), "base_footprint"), Pose);

      // In World coordinates
      geometry_msgs::Point PointWorld(FConversions::UToROS(Owner->PerceivedObject->PoseWorld.GetLocation()));
      geometry_msgs::Quaternion OrientationWorld(FConversions::UToROS(Owner->PerceivedObject->PoseWorld.GetRotation()));

      geometry_msgs::Pose PoseWorld(PointWorld, OrientationWorld);
      geometry_msgs::PoseStamped PoseWorldStamped(std_msgs::Header(Seq, FROSTime(), "map"), PoseWorld);


      urobosim_msgs::PerceiveObjectResult Result(Owner->PerceivedObject->Name, PoseStamped, PoseWorldStamped);
      ActionResult->SetResult(Result);

      Handler->PublishMsg(Topic, ActionResult);
      Handler->Process();

      Seq++ ;
      Owner->bPublishResult = false;

    }
}
