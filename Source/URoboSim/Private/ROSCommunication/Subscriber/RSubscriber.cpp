#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "Controller/RController.h"
// #include "geometry_msgs/Twist.h"
#include "Physics/RModel.h"

void URSubscriber::Init(UObject* InModel, TSharedPtr<FROSBridgeHandler> InHandler, FString InRosTopic )
{
  Model = Cast<ARModel>(InModel);
  ControllerComponent = Cast<URControllerComponent>(Model->Plugins["ControllerComponent"]);

  Init(InRosTopic);
  InHandler->AddSubscriber(Subscriber);

}

void URSubscriber::Init(FString RosTopic)
{
  if(!RosTopic.Equals(""))
    {
      Topic = RosTopic;
    }
  SetMessageType();
  CreateSubscriber();
}
