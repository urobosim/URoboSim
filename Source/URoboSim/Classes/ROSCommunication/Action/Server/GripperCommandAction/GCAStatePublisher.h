

#pragma once

#include "ROSCommunication/Action/Server/ActionStatePublisher.h"
#include "GCAStatePublisher.generated.h"

UCLASS()
class UROBOSIM_API URGripperCommandActionStatePublisher : public URActionStatePublisher
{
GENERATED_BODY()

protected:
virtual void SetOwner(UObject* InOwner){};

};
