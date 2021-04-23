
#pragma once

#include "ROSCommunication/Action/Server/ActionStatePublisher.h"
#include "PR2GCAStatePublisher.generated.h"

UCLASS()
class UROBOSIM_API URPR2GripperCommandActionStatePublisher : public URActionStatePublisher
{
  GENERATED_BODY()
protected:
  virtual void SetOwner(UObject* InOwner);

};
