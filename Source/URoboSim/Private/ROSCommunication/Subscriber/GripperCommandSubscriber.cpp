#include "ROSCommunication/Subscriber/GripperCommandSubscriber.h"
#include "Conversions.h"
#include "iai_wsg_50_msgs/msg/PositionCmd.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGripperCommandSubscriber, Log, All)

URGripperCommandSubscriber::URGripperCommandSubscriber()
{
  Topic = TEXT("/goal_position");
  MessageType = TEXT("iai_wsg_50_msgs/PositionCmd");
  JointControllerName = TEXT("JointController");
}

void URGripperCommandSubscriber::SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters)
{
  URGripperCommandSubscriberParameter *GripperCommandSubscriberParameters = Cast<URGripperCommandSubscriberParameter>(SubscriberParameters);
  if (GripperCommandSubscriberParameters)
  {
    Super::SetSubscriberParameters(SubscriberParameters);
    JointControllerName = GripperCommandSubscriberParameters->JointControllerName;
    GripperJointLeftName = GripperCommandSubscriberParameters->GripperJointLeftName;
    GripperJointRightName = GripperCommandSubscriberParameters->GripperJointRightName;
    Object = GripperCommandSubscriberParameters->Object;
  }
}

void URGripperCommandSubscriber::CreateSubscriber()
{
  if (GetOwner())
  {
    Subscriber = MakeShareable<FRGripperCommandSubscriberCallback>(
        new FRGripperCommandSubscriberCallback(Topic, MessageType, GetOwner()->GetController(JointControllerName), GripperJointLeftName, GripperJointRightName, Object));
  }
}

FRGripperCommandSubscriberCallback::FRGripperCommandSubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController, FString InGripperJointLeftName, FString InGripperJointRightName, TSoftObjectPtr<AActor> Object) : FROSBridgeSubscriber(InTopic, InType)
{
  JointController = Cast<URJointController>(InController);
  FEnableDrive EnableDrive;
  EnableDrive.PositionStrength = 1E5;
  EnableDrive.VelocityStrength = 1E4;
  EnableDrive.MaxForce = 1E10;
  GripperJointLeftName = InGripperJointLeftName;
  GripperJointRightName = InGripperJointRightName;
  // this->Object = Object;
  if (URJoint *Joint = JointController->GetOwner()->GetJoint(GripperJointLeftName))
  {
    JointController->DesiredJointStates.FindOrAdd(GripperJointLeftName);
    Joint->SetDrive(EnableDrive);
  }
  if (URJoint *Joint = JointController->GetOwner()->GetJoint(GripperJointRightName))
  {
    JointController->DesiredJointStates.FindOrAdd(GripperJointRightName);
    Joint->SetDrive(EnableDrive);
  }
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
  if (JointController)
  {
    TSharedPtr<iai_wsg_50_msgs::PositionCmd> GripperPosition = StaticCastSharedPtr<iai_wsg_50_msgs::PositionCmd>(Msg);
    float Pos = GripperPosition->GetPos();
    UE_LOG(LogRGripperCommandSubscriber, Warning, TEXT("Pos :%f"), Pos)
    if (JointController->DesiredJointStates.Contains(GripperJointLeftName))
    {
      JointController->DesiredJointStates[GripperJointLeftName].JointPosition = -Pos/2000.;
    }
    if (JointController->DesiredJointStates.Contains(GripperJointRightName))
    {
      JointController->DesiredJointStates[GripperJointRightName].JointPosition = Pos/1000.;
    }
    // if (Pos < 0.05)
    // {
    //   const FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);
    //   Object->AttachToComponent(JointController->GetOwner()->GetLink(TEXT("ur5_wrist_3_link"))->GetCollision(), AttachmentRules);
    // }
    // else
    // {
    //   Object->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    // }
  }
}
