#pragma once

#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "FTAGoalSubscriber.generated.h"

UCLASS()
class UROBOSIM_API URFollowJointTrajectoryActionGoalSubscriber: public URSubscriber
{
  GENERATED_BODY()

public:

  virtual void SetMessageType();
  virtual void CreateSubscriber();

};
