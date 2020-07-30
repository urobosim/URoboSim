
#pragma once

#include "ROSCommunication/RPublisher.h"
#include "Controller/RController.h"
#include "POAFeedbackPublisher.generated.h"

UCLASS()
class UROBOSIM_API URPerceiveObjectActionFeedbackPublisher : public URPublisher
{
    GENERATED_BODY()

public:

    virtual void Publish();

protected:
    virtual void SetMessageType();
    virtual void SetOwner(UObject* InOwner);

    UPROPERTY()
      URCameraController* Owner;
};
