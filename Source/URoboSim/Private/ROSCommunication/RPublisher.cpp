#include "ROSCommunication/RPublisher.h"
#include "sensor_msgs/JointState.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Quaternion.h"
#include "control_msgs/JointTrajectoryControllerState.h"
#include "actionlib_msgs/GoalStatusArray.h"
#include "control_msgs/FollowJointTrajectoryActionFeedback.h"
#include "control_msgs/FollowJointTrajectoryActionResult.h"
#include "control_msgs/FollowJointTrajectoryResult.h"
#include "pr2_controllers_msgs/PR2GripperCommandActionFeedback.h"
#include "pr2_controllers_msgs/PR2GripperCommandActionResult.h"
#include "pr2_controllers_msgs/PR2GripperCommandResult.h"
#include "pr2_controllers_msgs/PR2PointHeadActionFeedback.h"
#include "pr2_controllers_msgs/PR2PointHeadActionResult.h"
#include "pr2_controllers_msgs/PR2PointHeadResult.h"
#include "urobosim_msgs/PerceiveObjectActionResult.h"
#include "urobosim_msgs/PerceiveObjectActionFeedback.h"
#include "urobosim_msgs/PerceiveObjectResult.h"
#include "sensor_msgs/LaserScan.h"

#include "tf2_msgs/TFMessage.h"

URPublisher::URPublisher()
{
}

UROdomPublisher::UROdomPublisher()
{
  Topic = TEXT("/tf");
}
URJointStatePublisher::URJointStatePublisher()
{
  Topic = TEXT("/joint_states");
  JointParamTopic = TEXT("/hardware_interface/joints");
}
UR2DLidarPublisher::UR2DLidarPublisher()
{
  Topic = TEXT("/base_scan");
  UE_LOG(LogTemp, Log, TEXT("test"));
}

URJointTrajectoryControllerStatePublisher::URJointTrajectoryControllerStatePublisher()
{
  Topic = TEXT("/whole_body_controller/body/state");
}
URJointTrajectoryFeedbackPublisher::URJointTrajectoryFeedbackPublisher()
{
  JointParamTopic = TEXT("/whole_body_controller/joints");
}

void URPublisher::DeInit()
{
  Handler->Disconnect();
}

void URPublisher::Init(FString InHostIp, uint32 InPort, UObject* InOwner)
{
  Seq = 0;
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(InHostIp, InPort));
  Handler->Connect();
  SetMessageType();
  SetOwner(InOwner);
  CreatePublisher();
  Handler->AddPublisher(Publisher);
}

void URPublisher::Init(UObject* InOwner, TSharedPtr<FROSBridgeHandler> InHandler, FString InRosTopic)
{
  Seq = 0;
  Handler = InHandler;
  if(!InRosTopic.Equals(""))
    {
      Topic = InRosTopic;
    }
  SetMessageType();
  SetOwner(InOwner);
  CreatePublisher();
  Handler->AddPublisher(Publisher);
}

void URPublisher::CreatePublisher()
{
  Publisher = MakeShareable<FROSBridgePublisher>
    (new FROSBridgePublisher(Topic, MessageType));

  if(Publisher.IsValid())
    {
      UE_LOG(LogTemp, Log, TEXT("Publisher connected to RosBridge"));
    }
}

void URJointTrajectoryControllerStatePublisher::SetMessageType()
{
  MessageType = TEXT("control_msgs/JointTrajectoryControllerState");
}

void URFollowTrajectoryActionResultPublisher::SetMessageType()
{
  MessageType = TEXT("control_msgs/FollowJointTrajectoryActionResult");
}

void URJointTrajectoryFeedbackPublisher::SetMessageType()
{
  MessageType = TEXT("control_msgs/FollowJointTrajectoryActionFeedback");
}

void URJointStatePublisher::SetMessageType()
{
  MessageType = TEXT("sensor_msgs/JointState");
}

void URActionStatePublisher::SetMessageType()
{
  MessageType = TEXT("actionlib_msgs/GoalStatusArray");
}

void UROdomPublisher::SetMessageType()
{
  MessageType = TEXT("tf2_msgs/TFMessage");
}

void URPR2HeadActionResultPublisher::SetMessageType()
{
  MessageType = TEXT("pr2_controllers_msgs/PointHeadActionResult");
}

void URPR2HeadActionFeedbackPublisher::SetMessageType()
{
  MessageType = TEXT("pr2_controllers_msgs/PointHeadActionFeedback");
}

// void URGripperCommandActionResultPublisher::SetMessageType()
// {
// 	MessageType = TEXT("control_msgs/GripperCommandActionResult");
// }
//
// void URGripperCommandActionFeedbackPublisher::SetMessageType()
// {
// 	MessageType = TEXT("control_msgs/GripperCommandActionFeedback");
// }

void URPR2GripperCommandActionResultPublisher::SetMessageType()
{
  MessageType = TEXT("pr2_controllers_msgs/Pr2GripperCommandActionResult");
}

void URPR2GripperCommandActionFeedbackPublisher::SetMessageType()
{
  MessageType = TEXT("pr2_controllers_msgs/Pr2GripperCommandActionFeedback");
}

void URPerceiveObjectActionFeedbackPublisher::SetMessageType()
{
  MessageType = TEXT("urobosim_msgs/PerceiveObjectActionFeedback");
}

void URPerceiveObjectActionResultPublisher::SetMessageType()
{
  MessageType = TEXT("urobosim_msgs/PerceiveObjectActionResult");
}

void UR2DLidarPublisher::SetMessageType()
{
  MessageType = TEXT("sensor_msgs/LaserScan");
}

void UR2DLidarPublisher::SetOwner(UObject* InOwner)
{
  TArray<UR2DLidar*> LidarList;
  Cast<ARModel>(InOwner)->GetComponents(LidarList, true);
  for(auto & Sensor : LidarList)
    {
      if(Sensor->GetName().Equals(SensorName))
        {
          Owner = Sensor;
        }
    }
  if(!Owner)
    {
      UE_LOG(LogTemp, Error, TEXT("No Sensor with Name %s found"), *SensorName);
    }
}

void URFollowTrajectoryActionResultPublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URJointController>(ControllerComp->Controller.ControllerList["JointController"]);
  /* Controller->ControllerList["JointController"]; */
}

void URJointTrajectoryFeedbackPublisher::SetOwner(UObject* InOwner)
{
  ARModel* Model =Cast<ARModel>(InOwner);
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Model->Plugins["ControllerComponent"]);
  Owner = Cast<URJointController>(ControllerComp->Controller.ControllerList["JointController"]);
  ConfigClient = NewObject<URJointStateConfigurationClient>(this);
  ConfigClient->JointParamTopic = JointParamTopic;
  ConfigClient->URROSClient::Init(InOwner, &Owner->TrajectoryStatus.JointNames, Handler);
}

void URPR2GripperCommandActionStatePublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URGripperController>(ControllerComp->Controller.ControllerList[ControllerName]);
  if(Owner)
    {
      UE_LOG(LogTemp, Error, TEXT("Action State Publisher Found"));
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Action State Publisher not Found"));
    }
}

void URFollowJointTrajectoryActionStatePublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URJointController>(ControllerComp->Controller.ControllerList["JointController"]);
  if(Owner)
    {
      UE_LOG(LogTemp, Error, TEXT("Action State Publisher Found"));
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Action State Publisher not Found"));
    }
}

void URPR2HeadActionStatePublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URHeadTrajectoryController>(ControllerComp->Controller.ControllerList["HeadController"]);
  if(Owner)
    {
      UE_LOG(LogTemp, Error, TEXT("Action State Publisher Found"));
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Action State Publisher not Found"));
    }
}

void URJointStatePublisher::SetOwner(UObject* InOwner)
{
  Owner = Cast<ARModel>(InOwner);
  ConfigClient = NewObject<URJointStateConfigurationClient>(this);
  ConfigClient->JointParamTopic = JointParamTopic;
  ConfigClient->URROSClient::Init(InOwner, &ListJointName, Handler);
}

void UROdomPublisher::SetOwner(UObject* InOwner)
{
  Owner = Cast<ARModel>(InOwner);
}

void URPerceiveObjectActionResultPublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URCameraController>(ControllerComp->Controller.ControllerList["CameraController"]);
}

void URPerceiveObjectActionStatePublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URCameraController>(ControllerComp->Controller.ControllerList["CameraController"]);
}

void URPerceiveObjectActionFeedbackPublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URCameraController>(ControllerComp->Controller.ControllerList["CameraController"]);
}

void URPR2HeadActionResultPublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URHeadTrajectoryController>(ControllerComp->Controller.ControllerList["HeadController"]);
}

void URPR2HeadActionFeedbackPublisher::SetOwner(UObject* InOwner)
{
  // Owner = Cast<ARModel>(InOwner);
  ARModel* Model =Cast<ARModel>(InOwner);
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Model->Plugins["ControllerComponent"]);
  Owner = Cast<URHeadTrajectoryController>(ControllerComp->Controller.ControllerList["HeadController"]);
}

void URPR2GripperCommandActionResultPublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URGripperController>(ControllerComp->Controller.ControllerList[ControllerName]);
}

void URPR2GripperCommandActionFeedbackPublisher::SetOwner(UObject* InOwner)
{
  // Owner = Cast<ARModel>(InOwner);
  ARModel* Model =Cast<ARModel>(InOwner);
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Model->Plugins["ControllerComponent"]);
  Owner = Cast<URGripperController>(ControllerComp->Controller.ControllerList[ControllerName]);
}

void URJointStatePublisher::CreatePublisher()
{
  Super::CreatePublisher();
}

void URJointStatePublisher::Publish()
{
  ListJointPosition.Empty();
  ListJointPosition.Reserve(ListJointName.Num());
  ListJointVelocity.Empty();
  ListJointVelocity.Reserve(ListJointName.Num());
  ListJointEffort.Empty();
  ListJointEffort.Reserve(ListJointName.Num());

  for (auto &JointName : ListJointName)
    {
      if(Owner->Joints.Contains(JointName))
        {
          URJoint* Joint = Owner->Joints[JointName];
          float JointPosition = Joint->GetEncoderValue();
          float JointVelocity = Joint->GetJointVelocity();

          ListJointPosition.Add(JointPosition);
          ListJointVelocity.Add(0.0);
          ListJointEffort.Add(0.0);
        }
      else
        {
          UE_LOG(LogTemp, Log, TEXT("Joint not in robot: %s"), *JointName);
        }
    }

  TSharedPtr<sensor_msgs::JointState> JointState =
    MakeShareable(new sensor_msgs::JointState());
  JointState->SetHeader(std_msgs::Header(Seq, FROSTime(), TEXT("0")));
  JointState->SetName(ListJointName);
  JointState->SetPosition(ListJointPosition);
  JointState->SetVelocity(ListJointVelocity);
  JointState->SetEffort(ListJointEffort);

  Handler->PublishMsg(Topic, JointState);

  Handler->Process();
  Seq++;
  // UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
}

void URFollowTrajectoryActionResultPublisher::Publish()
{
  /* UE_LOG(LogTemp, Error, TEXT("bPublishResult pub: %s"), Owner->bPublishResult ? TEXT("True") : TEXT("False")); */
  if(Owner->bPublishResult)
    {
      UE_LOG(LogTemp, Error, TEXT("Publish Result"));
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

void URActionStatePublisher::Publish()
{
  TSharedPtr<actionlib_msgs::GoalStatusArray> GoalStatusArrayMsg =
    MakeShareable(new actionlib_msgs::GoalStatusArray());

  GoalStatusArrayMsg->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

  TArray<actionlib_msgs::GoalStatus> GSArray;

  for(auto& StatusInfo : Owner->GoalStatusList)
    {
      actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, "");
      GSArray.Add(GS);
    }
  GoalStatusArrayMsg->SetStatusList(GSArray);

  Handler->PublishMsg(Topic, GoalStatusArrayMsg);
  Handler->Process();

  Seq++ ;
  // UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
}

void URJointTrajectoryControllerStatePublisher::Publish()
{
  TSharedPtr<control_msgs::FollowJointTrajectoryFeedback> Feedback =
    MakeShareable(new control_msgs::FollowJointTrajectoryFeedback());

  Feedback->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));


  trajectory_msgs::JointTrajectoryPoint DesiredMsg;
  DesiredMsg.SetPositions(Owner->TrajectoryStatus.Desired);
  Feedback->SetDesired(DesiredMsg);

  trajectory_msgs::JointTrajectoryPoint ActualMsg;
  ActualMsg.SetPositions(Owner->TrajectoryStatus.Position);
  Feedback->SetActual(ActualMsg);

  trajectory_msgs::JointTrajectoryPoint ErrorMsg;
  ErrorMsg.SetPositions(Owner->TrajectoryStatus.Error);
  Feedback->SetError(ErrorMsg);

  Feedback->SetJointNames(Owner->TrajectoryStatus.JointNames);
  Handler->PublishMsg(Topic, Feedback);
  Handler->Process();

  Seq += 1;
}

void URJointTrajectoryFeedbackPublisher::Publish()
{
  // if(Owner->bFollowTrajectory)
  if(Owner->GetState() == UJointControllerState::FollowJointTrajectory)
    {
      TSharedPtr<control_msgs::FollowJointTrajectoryActionFeedback> Feedback =
        MakeShareable(new control_msgs::FollowJointTrajectoryActionFeedback());

      FGoalStatusInfo StatusInfo = Owner->GoalStatusList.Last();
      actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(StatusInfo.Secs, StatusInfo.NSecs), StatusInfo.Id), StatusInfo.Status, "");
      Feedback->SetStatus(GS);
      Feedback->SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

      control_msgs::FollowJointTrajectoryFeedback FeedbackMsg;
      FeedbackMsg.SetHeader(std_msgs::Header(Seq, FROSTime(), ""));

      trajectory_msgs::JointTrajectoryPoint DesiredMsg;
      DesiredMsg.SetPositions(Owner->TrajectoryStatus.Desired);
      FeedbackMsg.SetDesired(DesiredMsg);

      trajectory_msgs::JointTrajectoryPoint ActualMsg;
      ActualMsg.SetPositions(Owner->TrajectoryStatus.Position);
      FeedbackMsg.SetActual(ActualMsg);

      trajectory_msgs::JointTrajectoryPoint ErrorMsg;
      ErrorMsg.SetPositions(Owner->TrajectoryStatus.Error);
      FeedbackMsg.SetError(ErrorMsg);

      FeedbackMsg.SetJointNames(Owner->TrajectoryStatus.JointNames);
      Feedback->SetFeedback(FeedbackMsg);
      Handler->PublishMsg(Topic, Feedback);
      Handler->Process();

      // UE_LOG(LogTemp, Log, TEXT("JointState = %s"), *JointState->ToString());
      Seq += 1;
    }
}


void UROdomPublisher::Publish()
{

  geometry_msgs::TransformStamped BaseFrame;
  BaseFrame.SetHeader(std_msgs::Header(Seq, FROSTime(), OdomFrameId));
  BaseFrame.SetChildFrameId(BaseFrameId);

  //TODO get the correct pos of base footprint
  FVector BasePose =Owner->GetActorLocation();
  BasePose.Z = 0.;

  geometry_msgs::Transform BaseTransfMsg(
                                         geometry_msgs::Vector3(FConversions::UToROS(BasePose - FrameTransform.GetLocation())),
                                         geometry_msgs::Quaternion(FConversions::UToROS(Owner->GetActorQuat() *  FrameTransform.GetRotation())));
  BaseFrame.SetTransform(BaseTransfMsg);

  geometry_msgs::TransformStamped OdomFrame;
  OdomFrame.SetHeader(std_msgs::Header(Seq, FROSTime(), MapFrameId));
  OdomFrame.SetChildFrameId(OdomFrameId);

  geometry_msgs::Transform OdomTransfMsg(
                                         geometry_msgs::Vector3(FConversions::UToROS(FrameTransform.GetLocation())),
                                         geometry_msgs::Quaternion(FConversions::UToROS(FrameTransform.GetRotation())));
  OdomFrame.SetTransform(OdomTransfMsg);


  TSharedPtr<tf2_msgs::TFMessage> TfMessage =
    MakeShareable(new tf2_msgs::TFMessage());


  TfMessage->AddTransform(OdomFrame);
  TfMessage->AddTransform(BaseFrame);

  Seq += 1;

  Handler->PublishMsg(Topic, TfMessage);
  Handler->Process();
}

void URPR2HeadActionResultPublisher::Publish()
{
  if(Owner->bPublishResult)
    {
      UE_LOG(LogTemp, Error, TEXT("Publish Result"));
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
      UE_LOG(LogTemp, Error, TEXT("Publish Result"));
    }
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

void URPerceiveObjectActionFeedbackPublisher::Publish()
{}

void UR2DLidarPublisher::Publish()
{
  if(Owner->bPublishResult)
    {
      TSharedPtr<sensor_msgs::LaserScan> ScanData = MakeShareable(new sensor_msgs::LaserScan());
      ScanData->SetHeader(std_msgs::Header(Seq, FROSTime(), "base_laser_link"));
      ScanData->SetAngleMin(Owner->ScanAngleMin);
      ScanData->SetAngleMax(Owner->ScanAngleMax);
      ScanData->SetAngleIncrement(Owner->AngularIncrement);
      ScanData->SetRangeMin(Owner->MinimumDistance /100.);
      ScanData->SetRangeMax(Owner->MaximumDistance /100.);
      ScanData->SetTimeIncrement(Owner->TimeIncrement);
      ScanData->SetScanTime(Owner->ScanTime);

      TArray<float> Measurments = Owner->GetMeasurementData();
      // UE_LOG(LogTemp, Error, TEXT("Message Length %d"), Measurments.Num());
      ScanData->SetRanges(Measurments);
      Handler->PublishMsg(Topic, ScanData);
      Handler->Process();


      Seq++ ;
      Owner->bPublishResult = false;
    }
}

void URPerceiveObjectActionResultPublisher::Publish()
{
  if(Owner->bPublishResult)
    {
      UE_LOG(LogTemp, Error, TEXT("Publish Result"));
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
