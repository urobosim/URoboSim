
#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "Controller/ControllerType/JointController/RHeadController.h"
#include "PR2HAResultPublisher.generated.h"

UCLASS()
class UROBOSIM_API URPR2HeadActionResultPublisher : public URPublisher
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
