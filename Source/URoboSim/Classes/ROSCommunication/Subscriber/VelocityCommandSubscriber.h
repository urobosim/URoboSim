#pragma once

#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "VelocityCommandSubscriber.generated.h"

UCLASS()
class UROBOSIM_API URVelocityCommandSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};
