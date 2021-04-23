
#include "ROSCommunication/Actions/PerceiveObjectAction/POAFeedbackPublisher.h"
#include "urobosim_msgs/PerceiveObjectActionFeedback.h"

void URPerceiveObjectActionFeedbackPublisher::SetMessageType()
{
  MessageType = TEXT("urobosim_msgs/PerceiveObjectActionFeedback");
}

void URPerceiveObjectActionFeedbackPublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URCameraController>(ControllerComp->Controller.ControllerList["CameraController"]);
}

void URPerceiveObjectActionFeedbackPublisher::Publish()
{}
