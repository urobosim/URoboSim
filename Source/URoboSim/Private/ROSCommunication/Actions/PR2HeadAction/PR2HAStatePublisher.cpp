#include "ROSCommunication/Actions/PR2HeadAction/PR2HAStatePublisher.h"

void URPR2HeadActionStatePublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URHeadTrajectoryController>(ControllerComp->Controller.ControllerList["HeadController"]);
  if(Owner)
    {
      UE_LOG(LogTemp, Error, TEXT("Action State Publisher Found"));
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Action State Publisher not Found"));
    }
}
