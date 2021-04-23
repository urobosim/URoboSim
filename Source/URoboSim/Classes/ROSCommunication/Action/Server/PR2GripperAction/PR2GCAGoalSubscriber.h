#pragma once

#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "PR2GCAGoalSubscriber.generated.h"


UCLASS()
class UROBOSIM_API URPR2GripperCommandActionGoalSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};
