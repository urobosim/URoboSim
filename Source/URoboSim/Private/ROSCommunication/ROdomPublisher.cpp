#include "ROSCommunication/ROdomPublisher.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/Quaternion.h"
#include "geometry_msgs/Point.h"
#include "tf2_msgs/TFMessage.h"

UROdomPublisher::UROdomPublisher()
{
  Topic = TEXT("/tf");
}

void UROdomPublisher::SetMessageType()
{
  MessageType = TEXT("tf2_msgs/TFMessage");
}

void UROdomPublisher::SetOwner(UObject* InOwner)
{
  Owner = Cast<ARModel>(InOwner);
}

void UROdomPublisher::Publish()
{

  geometry_msgs::TransformStamped BaseFrame;
  BaseFrame.SetHeader(std_msgs::Header(Seq, FROSTime(), OdomFrameId));
  BaseFrame.SetChildFrameId(BaseFrameId);

  //TODO get the correct pos of base footprint
  FVector BasePose =Owner->GetActorLocation();
  if(bProjectToGround)
    {
      BasePose.Z = 0.;
    }

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
