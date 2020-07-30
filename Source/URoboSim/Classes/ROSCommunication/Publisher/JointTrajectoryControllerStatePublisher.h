
#pragma once

#include "ROSCommunication/Actions/FollowTrajectoryAction/FTAFeedbackPublisher.h"
#include "JointTrajectoryControllerStatePublisher.generated.h"

UCLASS()
class UROBOSIM_API URJointTrajectoryControllerStatePublisher : public URJointTrajectoryFeedbackPublisher
{
  GENERATED_BODY()
public:

    URJointTrajectoryControllerStatePublisher();
    virtual void Publish();

protected:
    virtual void SetMessageType();
};
