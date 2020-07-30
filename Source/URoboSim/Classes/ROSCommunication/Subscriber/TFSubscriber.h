#pragma once

#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "TFSubscriber.generated.h"

UCLASS()
class UROBOSIM_API URTFSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};
