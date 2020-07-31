#include "ROSCommunication/Subscriber/OdomReplaySubscriberCallback.h"
#include "Conversions.h"
#include "tf2_msgs/TFMessage.h"

FROSOdomReplaySubscriberCallback::FROSOdomReplaySubscriberCallback(
                                                                   const FString& InTopic, const FString& InType, UObject* InController) :
  FROSBridgeSubscriber(InTopic, InType)
{
  BaseController = Cast<URBaseController>(InController);
}

TSharedPtr<FROSBridgeMsg> FROSOdomReplaySubscriberCallback::ParseMessage
(TSharedPtr<FJsonObject> JsonObject) const
{
  // MessageType = TEXT("tf2_msgs/TFMessage");
  TSharedPtr<tf2_msgs::TFMessage> JointStateMessage =
        MakeShareable<tf2_msgs::TFMessage>(new tf2_msgs::TFMessage());

  JointStateMessage->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(JointStateMessage);
}

void FROSOdomReplaySubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if(BaseController)
    {
      TSharedPtr<tf2_msgs::TFMessage> TFMessage = StaticCastSharedPtr<tf2_msgs::TFMessage>(Msg);
      for(auto & TF : TFMessage->GetTransforms())
      // for(int i = 0; i < TFMessage.Num(); i++)
        {
          FString FrameName = TF.GetChildFrameId();
          if(FrameName.Equals(BaseController->BaseName))
            {
              FString ParentFrame = TF.GetHeader().GetFrameId();
              if(!ParentFrame.Equals("map"))
                {
                  UE_LOG(LogTemp, Error, TEXT("Frame not in map coordinates"));
                  return;
                }

              BaseController->SetLocationAndRotation(FConversions::ROSToU(TF.GetTransform().GetTranslation().GetVector()),
                                                     FConversions::ROSToU(TF.GetTransform().GetRotation().GetQuat()).Rotator());
            }
        }
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("BaseController not found"));
    }
}
