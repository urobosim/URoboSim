#include "ROSCommunication/Action/Server/ActionCancelSubscriber.h"
#include "actionlib_msgs/GoalID.h"

DEFINE_LOG_CATEGORY_STATIC(LogRActionCancelSubscriber, Log, All)

URActionCancelSubscriber::URActionCancelSubscriber()
{
  MessageType = TEXT("actionlib_msgs/GoalID");
}

void URActionCancelSubscriber::CreateSubscriber()
{
  if (GetOwner())
  {
    Subscriber = MakeShareable<FActionCancelSubscriberCallback>(
        new FActionCancelSubscriberCallback(Topic, MessageType, Controller));
  }
}

FActionCancelSubscriberCallback::FActionCancelSubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  Controller = Cast<URController>(InController);
}

void FActionCancelSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (Controller)
  {
    Controller->bCancel = true;
  }
  else
  {
    UE_LOG(LogRActionCancelSubscriber, Error, TEXT("Controller not found"));
  }
}

TSharedPtr<FROSBridgeMsg> FActionCancelSubscriberCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<actionlib_msgs::GoalID> GoalId =
      MakeShareable<actionlib_msgs::GoalID>(new actionlib_msgs::GoalID());

  GoalId->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(GoalId);
}