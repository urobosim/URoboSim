#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAGoalSubscriber.h"
#include "urobosim_msgs/PerceiveObjectActionGoal.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPOAGoalSubscriber, Log, All)

URPOAGoalSubscriber::URPOAGoalSubscriber()
{
  MessageType = TEXT("urobosim_msgs/PerceiveObjectActionGoal");
}

void URPOAGoalSubscriber::CreateSubscriber()
{
  Subscriber = MakeShareable<FPOAGoalSubscriberCallback>(
      new FPOAGoalSubscriberCallback(Topic, MessageType, Controller));
  if (Subscriber.IsValid())
  {
    UE_LOG(LogRPOAGoalSubscriber, Log, TEXT("Subscriber connected to RosBridge"));
  }
}

FPOAGoalSubscriberCallback::FPOAGoalSubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  CameraController = Cast<URCameraController>(InController);
}

TSharedPtr<FROSBridgeMsg> FPOAGoalSubscriberCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<urobosim_msgs::PerceiveObjectActionGoal> Message =
      MakeShareable<urobosim_msgs::PerceiveObjectActionGoal>(new urobosim_msgs::PerceiveObjectActionGoal());

  Message->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(Message);
}

void FPOAGoalSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (CameraController)
  {
    TSharedPtr<urobosim_msgs::PerceiveObjectActionGoal> Command = StaticCastSharedPtr<urobosim_msgs::PerceiveObjectActionGoal>(Msg);
    actionlib_msgs::GoalID Id = Command->GetGoalId();
    CameraController->AddGoalStatus(FGoalStatusInfo(Id.GetId(), Id.GetStamp().Secs, Id.GetStamp().NSecs));
    CameraController->TypeToPerceive = Command->GetGoal().GetType();
    CameraController->bActive = true;
    AsyncTask(ENamedThreads::GameThread, [this]() {
      // // Controller->Camera->UpdateImage();
      CameraController->PerceiveObject();
    });
  }
}