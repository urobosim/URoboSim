#include "ROSCommunication/Subscriber/RVelocityCommandSubscriber.h"
#include "Conversions.h"
#include "geometry_msgs/Twist.h"

DEFINE_LOG_CATEGORY_STATIC(LogRVelocityCommandSubscriber, Log, All)

URVelocityCommandSubscriber::URVelocityCommandSubscriber()
{
  Topic = TEXT("/base_controller/command");
  MessageType = TEXT("geometry_msgs/Twist");
}

void URVelocityCommandSubscriber::Init()
{
  if (GetOwner())
  {
    ControllerComponent = Cast<URControllerComponent>(GetOwner()->GetPlugin(TEXT("ControllerComponent")));
  }
}

void URVelocityCommandSubscriber::CreateSubscriber()
{
  if (ControllerComponent)
  {
    Subscriber = MakeShareable<FRVelocityCommandSubscriberCallback>(
        new FRVelocityCommandSubscriberCallback(Topic, MessageType, ControllerComponent->GetController(TEXT("BaseController"))));
  }
  else
  {
    UE_LOG(LogRVelocityCommandSubscriber, Error, TEXT("ControllerComponent not found in %s"), *GetName())
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
    FVector LinearVelocity = FConversions::ROSToU(VelocityCommand->GetLinear().GetVector());
    FVector AngularVelocity = VelocityCommand->GetAngular().GetVector();
    BaseController->MoveLinear(LinearVelocity);
    BaseController->MoveAngular(-FMath::RadiansToDegrees(AngularVelocity.Z));
  }
}