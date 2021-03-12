#include "ROSCommunication/Publisher/RTFPublisher.h"
#include "geometry_msgs/TransformStamped.h"
#include "geometry_msgs/Quaternion.h"
#include "geometry_msgs/Point.h"
#include "tf2_msgs/TFMessage.h"

URTFPublisher::URTFPublisher()
{
  Topic = TEXT("/tf");
}

void URTFPublisher::SetMessageType()
{
  UE_LOG(LogTemp, Error, TEXT("Init TFPublisher"));
  MessageType = TEXT("tf2_msgs/TFMessage");
}

void URTFPublisher::SetOwner(UObject* InOwner)
{
}

void URTFPublisher::AddObject(AActor* InObject)
{
  if(!ObjectList.Contains(InObject))
  {
    ObjectList.Add(InObject);
  }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Object already in list %s"), *InObject->GetName());
    }
}

void URTFPublisher::SetObjects(TArray<AActor*> InObject)
{
  ObjectList.Empty();
  ObjectList.Append(InObject);
}

void URTFPublisher::Publish()
{
  TSharedPtr<tf2_msgs::TFMessage> TfMessage =
    MakeShareable(new tf2_msgs::TFMessage());

  geometry_msgs::Transform ObjectTransfMsg;
  geometry_msgs::TransformStamped ObjectFrame;
  for(auto & Object : ObjectList)
    {
      UE_LOG(LogTemp, Error, TEXT("tf publish %s"), *Object->GetName());
      ObjectFrame.SetHeader(std_msgs::Header(Seq, FROSTime(), MapFrameId));
      ObjectFrame.SetChildFrameId(Object->GetName());

      geometry_msgs::Transform TransfMsgTemp(
                                           geometry_msgs::Vector3(FConversions::UToROS(Object->GetActorLocation())),
                                           geometry_msgs::Quaternion(FConversions::UToROS(Object->GetActorQuat() )));
      ObjectTransfMsg = TransfMsgTemp;
      ObjectFrame.SetTransform(ObjectTransfMsg);
      TfMessage->AddTransform(ObjectFrame);
    }

  // AActor* Object = ObjectList;
  // Seq += 1;


  Handler->PublishMsg(Topic, TfMessage);
  Handler->Process();
}
