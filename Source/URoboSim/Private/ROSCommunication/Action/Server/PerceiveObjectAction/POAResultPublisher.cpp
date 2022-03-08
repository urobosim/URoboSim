#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAResultPublisher.h"
#include "TFPublisher.h"
#include "Conversions.h"
#include "urobosim_msgs/PerceiveObjectActionResult.h"
#include "urobosim_msgs/PerceiveObjectResult.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPOAResultPublisher, Log, All)

URPOAResultPublisher::URPOAResultPublisher()
{
  MessageType = TEXT("urobosim_msgs/PerceiveObjectActionResult");
}

void URPOAResultPublisher::Init()
{
  Super::Init();
  CameraController = Cast<URCameraController>(Controller);

  for (TObjectIterator<ATFPublisher> Itr; Itr; ++Itr)
  {
    // Make sure the object is in the world
    if (GetWorld()->ContainsActor((*Itr)))
    {
      UE_LOG(LogRPOAResultPublisher, Warning, TEXT("Bind TFPublisher"));
      OnObjectDetected.AddUObject(*Itr, &ATFPublisher::AddObject);
    }
  }
  // TFPublisher = NewObject<URTFPublisher>(this, FName(*(GetName() + TEXT("_TFPublisher"))));
  // TFPublisher->Init(TEXT("127.0.0.1"), 9090, this);
}

void URPOAResultPublisher::Publish()
{
  if (CameraController && CameraController->bPublishResult)
  {
    static int Seq = 0;

    // TFPublisher->SetObjects(CameraController->PerceivedActors);
    // TFPublisher->Publish();
    for (auto &PerceivedActor : CameraController->PerceivedActors)
    {
      OnObjectDetected.Broadcast(PerceivedActor);
    }

    TSharedPtr<urobosim_msgs::PerceiveObjectActionResult> ActionResult =
        MakeShareable(new urobosim_msgs::PerceiveObjectActionResult());

    ActionResult->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

    // uint8 status = Owner->Status;
    FGoalStatusInfo StatusInfo = CameraController->GetGoalStatusList().Last();
    actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, "");
    ActionResult->SetStatus(GS);

    // In Base Footprint
    geometry_msgs::Point Point(FConversions::UToROS(CameraController->PerceivedObject->Pose.GetLocation()));
    geometry_msgs::Quaternion Orientation(FConversions::UToROS(CameraController->PerceivedObject->Pose.GetRotation()));

    geometry_msgs::Pose Pose(Point, Orientation);
    geometry_msgs::PoseStamped PoseStamped(std_msgs::Header(Seq, FROSTime(), "base_footprint"), Pose);

    // In World coordinates
    geometry_msgs::Point PointWorld(FConversions::UToROS(CameraController->PerceivedObject->PoseWorld.GetLocation()));
    geometry_msgs::Quaternion OrientationWorld(FConversions::UToROS(CameraController->PerceivedObject->PoseWorld.GetRotation()));

    geometry_msgs::Pose PoseWorld(PointWorld, OrientationWorld);
    geometry_msgs::PoseStamped PoseWorldStamped(std_msgs::Header(Seq, FROSTime(), "map"), PoseWorld);

    urobosim_msgs::PerceiveObjectResult Result(CameraController->PerceivedObject->Name, PoseStamped, PoseWorldStamped);
    ActionResult->SetResult(Result);

    Handler->PublishMsg(Topic, ActionResult);
    Handler->Process();

    Seq++;
    CameraController->bPublishResult = false;
  }
}
