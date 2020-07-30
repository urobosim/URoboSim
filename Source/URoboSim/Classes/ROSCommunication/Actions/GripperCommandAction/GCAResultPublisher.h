#pragma once

#include "ROSCommunication/RPublisher.h"
#include "Controller/RGripperController.h"
#include "GCAResultPublisher.generated.h"

UCLASS()
class UROBOSIM_API URGripperCommandActionResultPublisher : public URPublisher
{
    GENERATED_BODY()

public:

    virtual void Publish(){};

protected:
    virtual void SetMessageType(){};
    virtual void SetOwner(UObject* InOwner){};

    UPROPERTY()
      URGripperController* Owner;
};
