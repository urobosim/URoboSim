
#include "ROSCommunication/Action/Server/PerceiveObjectAction/POAStatePublisher.h"

// void URPerceiveObjectActionResultPublisher::SetMessageType()
// {
//   MessageType = TEXT("urobosim_msgs/PerceiveObjectActionResult");
// }

void URPerceiveObjectActionStatePublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URCameraController>(ControllerComp->Controller.ControllerList["CameraController"]);
}
