#include "ROSCommunication/Action/Server/FollowTrajectoryAction/FTAStatePublisher.h"

void URFollowJointTrajectoryActionStatePublisher::SetOwner(UObject* InOwner)
{
  URControllerComponent* ControllerComp = Cast<URControllerComponent>(Cast<ARModel>(InOwner)->Plugins["ControllerComponent"]);
  Owner = Cast<URJointController>(ControllerComp->Controller.ControllerList["JointController"]);
  if(Owner)
    {
      UE_LOG(LogTemp, Error, TEXT("Action State Publisher Found"));
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Action State Publisher not Found"));
    }
}
