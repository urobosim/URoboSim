#include "ROSCommunication/Publisher/RTFPublisher.h"
#include "Conversions.h"
#include "geometry_msgs/Point.h"
#include "geometry_msgs/Quaternion.h"
#include "geometry_msgs/TransformStamped.h"
#include "tf2_msgs/TFMessage.h"

DEFINE_LOG_CATEGORY_STATIC(LogRTFPublisher, Log, All)

URTFPublisher::URTFPublisher()
{
  Topic = TEXT("/tf");
  MessageType = TEXT("tf2_msgs/TFMessage");
  MapFrameId = TEXT("map");
}

void URTFPublisher::SetPublishParameters(URPublisherParameter *&PublisherParameters)
{
  if (URTFPublisherParameter *TFPublisherParameters = Cast<URTFPublisherParameter>(PublisherParameters))
  {
    Super::SetPublishParameters(PublisherParameters);
    MapFrameId = TFPublisherParameters->MapFrameId;
  }
}

void URTFPublisher::AddObject(AActor *InObject)
{
  if (!Objects.Contains(InObject))
  {
    Objects.Add(InObject);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Object already in list %s"), *InObject->GetName());
  }
}

void URTFPublisher::SetObjects(TArray<AActor *> InObjects)
{
  Objects = InObjects;
}

void URTFPublisher::Publish()
{
  static int Seq = 0;
  TSharedPtr<tf2_msgs::TFMessage> TfMessage =
      MakeShareable(new tf2_msgs::TFMessage());

  geometry_msgs::Transform ObjectTransfMsg;
  geometry_msgs::TransformStamped ObjectFrame;
  for (AActor *&Object : Objects)
  {
    UE_LOG(LogRTFPublisher, Error, TEXT("tf publish %s"), *Object->GetName());
    ObjectFrame.SetHeader(std_msgs::Header(Seq, FROSTime(), MapFrameId));
    ObjectFrame.SetChildFrameId(Object->GetName());

    geometry_msgs::Transform TransfMsgTemp(
        geometry_msgs::Vector3(FConversions::UToROS(Object->GetActorLocation())),
        geometry_msgs::Quaternion(FConversions::UToROS(Object->GetActorQuat())));
    ObjectTransfMsg = TransfMsgTemp;
    ObjectFrame.SetTransform(ObjectTransfMsg);
    TfMessage->AddTransform(ObjectFrame);
  }

  // AActor* Object = ObjectList;
  // Seq += 1;

  Handler->PublishMsg(Topic, TfMessage);
  Handler->Process();
}
