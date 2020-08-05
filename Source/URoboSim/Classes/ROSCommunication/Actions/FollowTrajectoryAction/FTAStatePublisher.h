#pragma once

#include "ROSCommunication/Actions/ActionStatePublisher.h"
#include "FTAStatePublisher.generated.h"

UCLASS()
class UROBOSIM_API URFollowJointTrajectoryActionStatePublisher : public URActionStatePublisher
{
  GENERATED_BODY()

protected:
    virtual void SetOwner(UObject* InOwner);
};
