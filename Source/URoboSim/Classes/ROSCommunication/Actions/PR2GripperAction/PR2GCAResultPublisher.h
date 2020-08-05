
#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "Controller/RGripperController.h"
#include "PR2GCAResultPublisher.generated.h"

UCLASS()
class UROBOSIM_API URPR2GripperCommandActionResultPublisher : public URPublisher
{
  GENERATED_BODY()

public:

  virtual void Publish();

protected:

  virtual void SetMessageType();
  virtual void SetOwner(UObject* InOwner);

  UPROPERTY()
    URGripperController* Owner;
};
