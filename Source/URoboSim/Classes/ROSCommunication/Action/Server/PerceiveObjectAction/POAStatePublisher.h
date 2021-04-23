
#pragma once

#include "ROSCommunication/Action/Server/ActionStatePublisher.h"
#include "POAStatePublisher.generated.h"

UCLASS()
class UROBOSIM_API URPerceiveObjectActionStatePublisher : public URActionStatePublisher
{
  GENERATED_BODY()

protected:

    virtual void SetOwner(UObject* InOwner);

};
