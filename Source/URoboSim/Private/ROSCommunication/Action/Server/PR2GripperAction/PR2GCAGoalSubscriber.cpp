#include "ROSCommunication/Action/Server/PR2GripperAction/PR2GCAGoalSubscriber.h"
#include "pr2_controllers_msgs/PR2GripperCommandActionGoal.h"

DEFINE_LOG_CATEGORY_STATIC(LogRPR2GCAGoalSubscriber, Log, All)

URPR2GCAGoalSubscriber::URPR2GCAGoalSubscriber()
{
  MessageType = TEXT("pr2_controllers_msgs/Pr2GripperCommandActionGoal");
}

void URPR2GCAGoalSubscriber::CreateSubscriber()
{
  UE_LOG(LogRPR2GCAGoalSubscriber, Warning, TEXT("%s"), *Controller->GetName())

  Subscriber = MakeShareable<FRPR2GCAGoalSubscriberCallback>(
      new FRPR2GCAGoalSubscriberCallback(Topic, MessageType, Controller));
  if (Subscriber.IsValid())
  {
    UE_LOG(LogRPR2GCAGoalSubscriber, Log, TEXT("Subscriber connected to RosBridge"));
  }
}

FRPR2GCAGoalSubscriberCallback::FRPR2GCAGoalSubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  GripperController = Cast<URGripperController>(InController);
}

TSharedPtr<FROSBridgeMsg> FRPR2GCAGoalSubscriberCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionGoal> Message =
      MakeShareable<pr2_controllers_msgs::PR2GripperCommandActionGoal>(new pr2_controllers_msgs::PR2GripperCommandActionGoal());

  Message->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(Message);
}

void FRPR2GCAGoalSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (GripperController)
  {
    TSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionGoal> Command = StaticCastSharedPtr<pr2_controllers_msgs::PR2GripperCommandActionGoal>(Msg);

    actionlib_msgs::GoalID Id = Command->GetGoalId();

    AsyncTask(ENamedThreads::GameThread, [this, Id, Command]() {
      float Pos = Command->GetGoal().GetCommand().GetPosition() * 100.0;
      if (Pos > 8.5)
      {
        UE_LOG(LogTemp, Warning, TEXT("%s: DesiredPosition should not exceed 8.5"), *GripperController->GetName());
        Pos = 8.5;
      }

      GripperController->Position = Pos;

      GripperController->MaxEffort = Command->GetGoal().GetCommand().GetMaxEffort();
      UE_LOG(LogRPR2GCAGoalSubscriber, Log, TEXT("Received GripperCommand Id: %s Goal: %f"), *Id.GetId(), GripperController->Position);
      GripperController->AddGoalStatus(FGoalStatusInfo(Id.GetId(), Id.GetStamp().Secs, Id.GetStamp().NSecs));
      GripperController->UpdateGripper();
    });
  }
}