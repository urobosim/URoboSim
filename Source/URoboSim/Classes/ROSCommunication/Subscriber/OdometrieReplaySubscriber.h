#pragma once

#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "OdometrieReplaySubscriber.generated.h"

UCLASS()
class UROBOSIM_API UROdometrieReplaySubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};
