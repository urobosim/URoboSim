#include "ROSCommunication/Subscriber/RVelocityCommandSubscriber.h"
#include "Conversions.h"
#include "geometry_msgs/Twist.h"

DEFINE_LOG_CATEGORY_STATIC(LogRVelocityCommandSubscriber, Log, All)

void URVelocityCommandSubscriber::Init()
{
  if (!SubscriberParameters)
  {
    SubscriberParameters = CreateDefaultSubobject<URVelocityCommandSubscriberParameter>(TEXT("VelocityCommandSubscriberParameters"));
  }

  if (GetOwner())
  {
    ControllerComponent = Cast<URControllerComponent>(GetOwner()->GetPlugin(TEXT("ControllerComponent")));
  }
}

void URVelocityCommandSubscriber::CreateSubscriber()
{
  if (ControllerComponent)
  {
    if (Cast<URVelocityCommandSubscriberParameter>(SubscriberParameters))
    {
      Subscriber = MakeShareable<FRVelocityCommandSubscriberCallback>(
          new FRVelocityCommandSubscriberCallback(SubscriberParameters->Topic, SubscriberParameters->MessageType, ControllerComponent->GetController(TEXT("BaseController"))));
    }
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