#include "ROSCommunication/Action/Server/ActionCancelCallback.h"
#include "actionlib_msgs/GoalID.h"

FROSActionCancelCallback::FROSActionCancelCallback(
	const FString& InTopic, const FString& InType, UObject* InController) :
	FROSBridgeSubscriber(InTopic, InType)
{
	Controller = Cast<URController>(InController);
}

FROSActionCancelCallback::~FROSActionCancelCallback()
{
}

void FROSActionCancelCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if(Controller)
    {
      Controller->bCancel = true;
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("JonintController not found"));
    }
}

TSharedPtr<FROSBridgeMsg> FROSActionCancelCallback::ParseMessage
(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<actionlib_msgs::GoalID> GoalId =
    MakeShareable<actionlib_msgs::GoalID>(new actionlib_msgs::GoalID());

  GoalId->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(GoalId);
}
