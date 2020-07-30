#pragma once

#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "JointStateReplaySubscriber.generated.h"

UCLASS()
class UROBOSIM_API URJointStateReplaySubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};
