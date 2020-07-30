
#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "Controller/RHeadController.h"
#include "PR2HAFeedbackPublisher.generated.h"

UCLASS()
class UROBOSIM_API URPR2HeadActionFeedbackPublisher : public URPublisher
{
    GENERATED_BODY()

public:

    virtual void Publish();
protected:
    virtual void SetMessageType();
    virtual void SetOwner(UObject* InOwner);

    UPROPERTY()
      URHeadTrajectoryController* Owner;
};
