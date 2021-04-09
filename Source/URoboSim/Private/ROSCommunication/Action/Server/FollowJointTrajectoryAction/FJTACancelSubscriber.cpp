#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTACancelSubscriber.h"
#include "actionlib_msgs/GoalID.h"

DEFINE_LOG_CATEGORY_STATIC(LogRFJTACancelSubscriber, Log, All)

URFJTACancelSubscriber::URFJTACancelSubscriber()
{
  SubscriberParameters = CreateDefaultSubobject<URActionCancelSubscriberParameter>(TEXT("FJTACancelSubscriberParameters"));
}

void URFJTACancelSubscriber::CreateSubscriber()
{
  if (ControllerComponent)
  {
    if (Cast<URActionCancelSubscriberParameter>(SubscriberParameters))
    {
      Subscriber = MakeShareable<FRFJTACancelSubscriberCallback>(
          new FRFJTACancelSubscriberCallback(SubscriberParameters->Topic, SubscriberParameters->MessageType, ControllerComponent->GetController(TEXT("JointController"))));
    }
  }
  else
  {
    UE_LOG(LogRFJTACancelSubscriber, Error, TEXT("ControllerComponent not found in %s"), *GetName())
  }
}

FRFJTACancelSubscriberCallback::FRFJTACancelSubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  JointController = Cast<URJointController>(InController);
}

void FRFJTACancelSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (JointController)
  {
    JointController->bCancel = true;
  }
  else
  {
    UE_LOG(LogRFJTACancelSubscriber, Error, TEXT("JointController not found"))
  }
}

TSharedPtr<FROSBridgeMsg> FRFJTACancelSubscriberCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<actionlib_msgs::GoalID> GoalId =
      MakeShareable<actionlib_msgs::GoalID>(new actionlib_msgs::GoalID());

  GoalId->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(GoalId);
}