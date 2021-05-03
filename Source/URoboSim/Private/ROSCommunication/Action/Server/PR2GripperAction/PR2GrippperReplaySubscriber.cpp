#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GripperReplaySubscriber.h"
#include "pr2_controllers_msgs/PR2GripperCommandActionGoal.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPR2GripperReplaySubscriber, Log, All)

URPR2GripperReplaySubscriber::URPR2GripperReplaySubscriber()
{
  MessageType = TEXT("pr2_controllers_msgs/Pr2GripperCommandActionGoal");
}

void URPR2GripperReplaySubscriber::CreateSubscriber()
{
  UE_LOG(LogRPR2GripperReplaySubscriber, Warning, TEXT("%s"), *Controller->GetName());

  Subscriber = MakeShareable<FRPR2GripperReplaySubscriberCallback>(
      new FRPR2GripperReplaySubscriberCallback(Topic, MessageType, Controller));
  if (Subscriber.IsValid())
  {
    UE_LOG(LogRPR2GripperReplaySubscriber, Log, TEXT("Subscriber connected to RosBridge"));
  }
}

FRPR2GripperReplaySubscriberCallback::FRPR2GripperReplaySubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  GripperController = Cast<URGripperController>(InController);
}

TSharedPtr<FROSBridgeMsg> FRPR2GripperReplaySubscriberCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionGoal> Message =
      MakeShareable<pr2_controllers_msgs::PR2GripperCommandActionGoal>(new pr2_controllers_msgs::PR2GripperCommandActionGoal());

  Message->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(Message);
}

void FRPR2GripperReplaySubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (GripperController)
  {
    TSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionGoal> Command = StaticCastSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionGoal>(Msg);

    actionlib_msgs::GoalID Id = Command->GetGoalId();

    AsyncTask(ENamedThreads::GameThread, [this, Id, Command]() {
      float Pos = Command->GetGoal().GetCommand().GetPosition() * 100.0;
      if (Pos > Position)
      {
        UE_LOG(LogRPR2GripperReplaySubscriber, Log, TEXT("Release"), GripperController->Position);
        GripperController->Release();
      }
      else
      {
        UE_LOG(LogRPR2GripperReplaySubscriber, Log, TEXT("Grasp"));
        GripperController->Grasp();
      }

      Position = Pos;
    });
  }
}
