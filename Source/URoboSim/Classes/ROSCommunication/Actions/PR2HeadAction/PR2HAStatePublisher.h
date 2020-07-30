#pragma once

#include "ROSCommunication/Actions/ActionStatePublisher.h"
#include "PR2HAStatePublisher.generated.h"

UCLASS()
class UROBOSIM_API URPR2HeadActionStatePublisher : public URActionStatePublisher
{
  GENERATED_BODY()

protected:

  virtual void SetOwner(UObject* InOwner);

};
