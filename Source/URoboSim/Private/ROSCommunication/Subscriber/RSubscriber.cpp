#include "ROSCommunication/Subscriber/RSubscriber.h"

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

void URSubscriber::SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters)
{
  if (SubscriberParameters)
  {
    Topic = SubscriberParameters->Topic;
    MessageType = SubscriberParameters->MessageType;
    ControllerName = SubscriberParameters->ControllerName;
  }
}