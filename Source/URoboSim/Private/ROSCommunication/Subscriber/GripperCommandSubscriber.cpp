#include "ROSCommunication/Subscriber/GripperCommandSubscriber.h"
#include "Conversions.h"
#include "iai_wsg_50_msgs/msg/PositionCmd.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGripperCommandSubscriber, Log, All)

URGripperCommandSubscriber::URGripperCommandSubscriber()
{
  Topic = TEXT("/goal_position");
  MessageType = TEXT("iai_wsg_50_msgs/PositionCmd");
  GripperControllerName = TEXT("?GripperController");
}

void URGripperCommandSubscriber::SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters)
{
  URGripperCommandSubscriberParameter *GripperCommandSubscriberParameters = Cast<URGripperCommandSubscriberParameter>(SubscriberParameters);
  if (GripperCommandSubscriberParameters)
  {
    Super::SetSubscriberParameters(SubscriberParameters);
    GripperControllerName = GripperCommandSubscriberParameters->GripperControllerName;
    // GripperJointLeftName = GripperCommandSubscriberParameters->GripperJointLeftName;
    // GripperJointRightName = GripperCommandSubscriberParameters->GripperJointRightName;
    // Object = GripperCommandSubscriberParameters->Object;
  }
}

void URGripperCommandSubscriber::CreateSubscriber()
{
  if (GetOwner())
  {
    Subscriber = MakeShareable<FRGripperCommandSubscriberCallback>(
        new FRGripperCommandSubscriberCallback(Topic, MessageType, GetOwner()->GetController(GripperControllerName)));
  }
}

FRGripperCommandSubscriberCallback::FRGripperCommandSubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  GripperController = Cast<URWSGGripperController>(InController);
  // FEnableDrive EnableDrive;
  // EnableDrive.PositionStrength = 1E5;
  // EnableDrive.VelocityStrength = 1E4;
  // EnableDrive.MaxForce = 1E10;
  // GripperJointLeftName = InGripperJointLeftName;
  // GripperJointRightName = InGripperJointRightName;
  // if (URJoint *Joint = JointController->GetOwner()->GetJoint(GripperJointLeftName))
  // {
  //   JointController->DesiredJointStates.FindOrAdd(GripperJointLeftName);
  //   Joint->SetDrive(EnableDrive);
  // }
  // if (URJoint *Joint = JointController->GetOwner()->GetJoint(GripperJointRightName))
  // {
  //   JointController->DesiredJointStates.FindOrAdd(GripperJointRightName);
  //   Joint->SetDrive(EnableDrive);
  // }
}

TSharedPtr<FROSBridgeMsg> FRGripperCommandSubscriberCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<iai_wsg_50_msgs::PositionCmd> GripperPosition =
      MakeShareable<iai_wsg_50_msgs::PositionCmd>(new iai_wsg_50_msgs::PositionCmd());

  GripperPosition->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(GripperPosition);
}

void FRGripperCommandSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (GripperController)
  {
    TSharedPtr<iai_wsg_50_msgs::PositionCmd> GripperPosition = StaticCastSharedPtr<iai_wsg_50_msgs::PositionCmd>(Msg);
    float Pos = GripperPosition->GetPos();
    UE_LOG(LogRGripperCommandSubscriber, Log, TEXT("Pos :%f"), Pos);
    GripperController->SetPose(Pos);
  }
}
