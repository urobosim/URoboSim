#include "ROSCommunication/Subscriber/VelocityCommandSubscriberCallback.h"
#include "geometry_msgs/Twist.h"

FROSVelocityCommandSubscriberCallback::FROSVelocityCommandSubscriberCallback(
                                        const FString& InTopic, const FString& InType, UObject* InController) :
                                        FROSBridgeSubscriber(InTopic, InType)
{
  BaseController = Cast<URBaseController>(InController);
}

FROSVelocityCommandSubscriberCallback::~FROSVelocityCommandSubscriberCallback()
{
}

TSharedPtr<FROSBridgeMsg> FROSVelocityCommandSubscriberCallback::ParseMessage
(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<geometry_msgs::Twist> VelocityCommand =
    MakeShareable<geometry_msgs::Twist>(new geometry_msgs::Twist());

  VelocityCommand->FromJson(JsonObject);

  return StaticCastSharedPtr<FROSBridgeMsg>(VelocityCommand);
}

void FROSVelocityCommandSubscriberCallback::Callback(TSharedPtr<FROSBridgeMsg> Msg)
{
  if(BaseController)
    {
      TSharedPtr<geometry_msgs::Twist> VelocityCommand = StaticCastSharedPtr<geometry_msgs::Twist>(Msg);
      FVector Linear = FConversions::ROSToU(VelocityCommand->GetLinear().GetVector());
      FVector Angular = VelocityCommand->GetAngular().GetVector();
      BaseController->MoveLinear(Linear);
      BaseController->Turn(Angular.Z);
    }
}
