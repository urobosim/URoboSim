#include "ROSCommunication/Subscriber/TFSubscriber.h"
#include "Conversions.h"
#include "tf2_msgs/TFMessage.h"

DEFINE_LOG_CATEGORY_STATIC(LogRTFSubscriber, Log, All)

URTFSubscriber::URTFSubscriber()
{
  MessageType = TEXT("tf2_msgs/TFMessage");
  TFControllerName = TEXT("TFController");
}

void URTFSubscriber::SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters)
{
  URTFSubscriberParameter *TFSubscribeParameters = Cast<URTFSubscriberParameter>(SubscriberParameters);
  if (TFSubscribeParameters)
  {
    Super::SetSubscriberParameters(SubscriberParameters);
    TFControllerName = TFSubscribeParameters->TFControllerName;
  }
}

void URTFSubscriber::CreateSubscriber()
{
  if (GetOwner())
  {
    Subscriber = MakeShareable<FRTFSubscriberCallback>(
        new FRTFSubscriberCallback(Topic, MessageType, GetOwner()->GetController(TFControllerName)));
  }
}

FRTFSubscriberCallback::FRTFSubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  TFController = Cast<URTFController>(InController);
}

TSharedPtr<FROSBridgeMsg> FRTFSubscriberCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<tf2_msgs::TFMessage> JointStateMessage =
      MakeShareable<tf2_msgs::TFMessage>(new tf2_msgs::TFMessage());

  JointStateMessage->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(JointStateMessage);
}

void FRTFSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (TFController)
  {
    TSharedPtr<tf2_msgs::TFMessage> TFMessage = StaticCastSharedPtr<tf2_msgs::TFMessage>(Msg);
    for (auto &TF : TFMessage->GetTransforms())
    {
      FString ChildFrame = TF.GetChildFrameId();
      FString ParentFrame = TF.GetHeader().GetFrameId();
      FTransform Pose = FTransform(FConversions::ROSToU(TF.GetTransform().GetRotation().GetQuat()),
                                   FConversions::ROSToU(TF.GetTransform().GetTranslation().GetVector()),
                                   FVector(1.0f, 1.0f, 1.0f));
      FTFInfo TFInfo;
      TFInfo.ParentFrame = ParentFrame;
      TFInfo.Pose = Pose;

      TFController->AddTF(ChildFrame, TFInfo);
    }
    TFController->UpdateFramePoses();
  }
  else
  {
    UE_LOG(LogRTFSubscriber, Error, TEXT("TFController not found"));
  }
}