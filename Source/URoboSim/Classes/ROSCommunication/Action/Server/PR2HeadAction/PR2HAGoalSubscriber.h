
#pragma once

#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "PR2HAGoalSubscriber.generated.h"

UCLASS()
class UROBOSIM_API URPR2HeadActionGoalSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};
