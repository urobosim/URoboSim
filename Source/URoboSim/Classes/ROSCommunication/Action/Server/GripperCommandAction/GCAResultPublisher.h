#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "Controller/ControllerType/JointController/RGripperController.h"
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
