#include "ROSCommunication/Actions/PR2GripperAction/PR2GCAStatePublisher.h"

void URPR2GripperCommandActionStatePublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URGripperController>(ControllerComp->Controller.ControllerList[ControllerName]);
  if(Owner)
    {
      UE_LOG(LogTemp, Error, TEXT("Action State Publisher Found"));
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Action State Publisher not Found"));
    }
}
