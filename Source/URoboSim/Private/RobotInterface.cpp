#include "RobotInterface.h"

DEFINE_LOG_CATEGORY_STATIC(LogRobotInterface, Log, All);

void IRobotInterface::SetOwner(UObject *InOwner)
{
  Owner = Cast<ARModel>(InOwner);
  if (!Owner)
    {
      if (Cast<URControllerComponent>(InOwner))
        {
          Owner = Cast<ARModel>(Cast<URControllerComponent>(InOwner)->GetOwner());
        }
      else
        {

        }
    }
  if (!Owner)
  {
    UE_LOG(LogRobotInterface, Error, TEXT("Owner of RobotInterface not found, Outer is %s"), *InOwner->GetName());
  }
}
