
#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "Controller/ControllerType/JointController/RJointController.h"
#include "FTAResultPublisher.generated.h"

UCLASS()
class UROBOSIM_API URFollowTrajectoryActionResultPublisher : public URPublisher
{
    GENERATED_BODY()

public:

    virtual void Publish();

protected:
    virtual void SetMessageType();
    virtual void SetOwner(UObject* InOwner);

    UPROPERTY()
      URJointController* Owner;

};
