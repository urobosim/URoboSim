
#pragma once

#include "ROSCommunication/RPublisher.h"
#include "Controller/RGripperController.h"
#include "GCAFeedbackPublisher.generated.h"

UCLASS()
class UROBOSIM_API URGripperCommandActionFeedbackPublisher : public URPublisher
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
