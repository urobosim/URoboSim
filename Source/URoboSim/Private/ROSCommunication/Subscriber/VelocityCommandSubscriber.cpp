#include "ROSCommunication/Subscriber/VelocityCommandSubscriber.h"
#include "Conversions.h"
#include "geometry_msgs/Twist.h"

DEFINE_LOG_CATEGORY_STATIC(LogRVelocityCommandSubscriber, Log, All)

URVelocityCommandSubscriber::URVelocityCommandSubscriber()
{
  Topic = TEXT("/base_controller/command");
  MessageType = TEXT("geometry_msgs/Twist");
  BaseControllerName = TEXT("BaseController");
}

void URVelocityCommandSubscriber::SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters)
{
  URVelocityCommandSubscriberParameter *VelocityCommandSubscriberParameters = Cast<URVelocityCommandSubscriberParameter>(SubscriberParameters);
  if (VelocityCommandSubscriberParameters)
  {
    Super::SetSubscriberParameters(SubscriberParameters);
    BaseControllerName = VelocityCommandSubscriberParameters->BaseControllerName;
  }
}

void URVelocityCommandSubscriber::CreateSubscriber()
{
  if (GetOwner())
  {
    Subscriber = MakeShareable<FRVelocityCommandSubscriberCallback>(
        new FRVelocityCommandSubscriberCallback(Topic, MessageType, GetOwner()->GetController(BaseControllerName)));
  }
}

FRVelocityCommandSubscriberCallback::FRVelocityCommandSubscriberCallback(
    const FString &InTopic, const FString &InType, UObject *InController) : FROSBridgeSubscriber(InTopic, InType)
{
  BaseController = Cast<URBaseController>(InController);
}

TSharedPtr<FROSBridgeMsg> FRVelocityCommandSubscriberCallback::ParseMessage(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<geometry_msgs::Twist> VelocityCommand =
      MakeShareable<geometry_msgs::Twist>(new geometry_msgs::Twist());

  VelocityCommand->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(VelocityCommand);
}

void FRVelocityCommandSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if (BaseController)
  {
    TSharedPtr<geometry_msgs::Twist> VelocityCommand = StaticCastSharedPtr<geometry_msgs::Twist>(Msg);
    FVector Linear = FConversions::ROSToU(VelocityCommand->GetLinear().GetVector());
    FVector Angular = VelocityCommand->GetAngular().GetVector();
    BaseController->MoveLinear(Linear);
    BaseController->Turn(Angular.Z);
  }
}
