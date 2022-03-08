#include "ROSCommunication/Subscriber/OdometrieReplaySubscriber.h"
#include "Conversions.h"
#include "tf2_msgs/TFMessage.h"

DEFINE_LOG_CATEGORY_STATIC(LogROdometrieReplaySubscriber, Log, All)

UROdometrieReplaySubscriber::UROdometrieReplaySubscriber()
{
  MessageType = TEXT("tf2_msgs/TFMessage");
  BaseControllerName = TEXT("BaseController");
}

void UROdometrieReplaySubscriber::SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters)
{
  UROdometrieReplaySubscriberParameter *OdometrieReplaySubscriberParameters = Cast<UROdometrieReplaySubscriberParameter>(SubscriberParameters);
  if (OdometrieReplaySubscriberParameters)
  {
    Super::SetSubscriberParameters(SubscriberParameters);
    BaseControllerName = OdometrieReplaySubscriberParameters->BaseControllerName;
  }
}

void UROdometrieReplaySubscriber::CreateSubscriber()
{
  if (GetOwner())
  {
    Subscriber = MakeShareable<FROdometrieReplaySubscriberCallback>(
        new FROdometrieReplaySubscriberCallback(Topic, MessageType, GetOwner()->GetController(BaseControllerName)));
  }
}

FROdometrieReplaySubscriberCallback::FROdometrieReplaySubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  BaseController = Cast<URBaseController>(InController);
}

TSharedPtr<FROSBridgeMsg> FROdometrieReplaySubscriberCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  // MessageType = TEXT("tf2_msgs/TFMessage");
  TSharedPtr<tf2_msgs::TFMessage> JointStateMessage =
      MakeShareable<tf2_msgs::TFMessage>(new tf2_msgs::TFMessage());

  JointStateMessage->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(JointStateMessage);
}

void FROdometrieReplaySubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (BaseController)
  {
    TSharedPtr<tf2_msgs::TFMessage> TFMessage = StaticCastSharedPtr<tf2_msgs::TFMessage>(Msg);
    for (auto &TF : TFMessage->GetTransforms())
    // for(int i = 0; i < TFMessage.Num(); i++)
    {
      FString FrameName = TF.GetChildFrameId();
      FrameName.RemoveFromStart(TEXT("/"));
      if (FrameName.Equals(BaseController->BaseName))
      {
        FString ParentFrame = TF.GetHeader().GetFrameId();
        ParentFrame.RemoveFromStart(TEXT("/"));
        if (!ParentFrame.Equals("map"))
        {
          UE_LOG(LogROdometrieReplaySubscriber, Error, TEXT("Frame not in map coordinates"));
          return;
        }

        BaseController->SetLocationAndRotation(FConversions::ROSToU(TF.GetTransform().GetTranslation().GetVector()),
                                               FConversions::ROSToU(TF.GetTransform().GetRotation().GetQuat()).Rotator());
      }
      else
      {
        UE_LOG(LogROdometrieReplaySubscriber, Error, TEXT("FrameName %s different from BaseName %s"), *FrameName, *BaseController->BaseName);
      }
    }
  }
  else
  {
    UE_LOG(LogROdometrieReplaySubscriber, Error, TEXT("BaseController not found"));
  }
}