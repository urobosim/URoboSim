#pragma once

#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "PR2GripperReplaySubscriber.generated.h"


UCLASS()
class UROBOSIM_API URPR2GripperReplaySubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};
