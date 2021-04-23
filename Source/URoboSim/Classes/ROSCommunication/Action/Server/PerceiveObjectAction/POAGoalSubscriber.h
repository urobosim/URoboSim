#pragma once

#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "POAGoalSubscriber.generated.h"

UCLASS()
class UROBOSIM_API URPerceiveObjectActionGoalSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};
