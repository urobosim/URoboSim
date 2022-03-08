#include "ROSCommunication/Publisher/ROdomPublisher.h"
#include "Conversions.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Quaternion.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2_msgs/TFMessage.h"

UROdomPublisher::UROdomPublisher()
{
  Topic = TEXT("/tf");
  MessageType = TEXT("tf2_msgs/TFMessage");
  MapFrameId = TEXT("map");
  OdomFrameId = TEXT("odom_combined");
  BaseFrameId = TEXT("base_footprint");
  bProjectToGround = true;
}

void UROdomPublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  if (UROdomPublisherParameter *OdomPublisherParameters = Cast<UROdomPublisherParameter>(PublisherParameters))
  {
    Super::SetPublishParameters(PublisherParameters);
    MapFrameId = OdomPublisherParameters->MapFrameId;
    OdomFrameId = OdomPublisherParameters->OdomFrameId;
    BaseFrameId = OdomPublisherParameters->BaseFrameId;
    bProjectToGround = OdomPublisherParameters->bProjectToGround;
  }
}

void UROdomPublisher::Publish()
{
  if (GetOwner())
  {
    static int Seq = 0;
    geometry_msgs::TransformStamped BaseFrame;
    BaseFrame.SetHeader(std_msgs::Header(Seq, FROSTime(), OdomFrameId));
    BaseFrame.SetChildFrameId(BaseFrameId);

    //TODO get the correct pos of base footprint
    FVector BasePose = GetOwner()->GetActorLocation();
    if (bProjectToGround)
    {
      BasePose.Z = 0.;
    }

    geometry_msgs::Transform BaseTransfMsg(
        geometry_msgs::Vector3(FConversions::UToROS(BasePose - FrameTransform.GetLocation())),
        geometry_msgs::Quaternion(FConversions::UToROS(GetOwner()->GetActorQuat() * FrameTransform.GetRotation())));
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

    Handler->PublishMsg(Topic, TfMessage);
    Handler->Process();

    Seq++;
  }
}
