
#pragma once

#include "ROSCommunication/RPublisher.h"
#include "Controller/RController.h"
#include "ActionStatePublisher.generated.h"

UCLASS()
class UROBOSIM_API URActionStatePublisher : public URPublisher
{
  GENERATED_BODY()

public:

  virtual void Publish();

protected:

  virtual void SetMessageType();
  virtual void SetOwner(UObject* InOwner){};

  UPROPERTY()
    URController* Owner;
};
