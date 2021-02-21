#pragma once

#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "JointStateSubscriber.generated.h"

UCLASS()
class UROBOSIM_API URJointStateSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};
