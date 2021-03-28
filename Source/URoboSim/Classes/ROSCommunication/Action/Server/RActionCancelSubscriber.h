#pragma once

#include "Controller/RControllerComponent.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
// clang-format off
#include "RActionCancelSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URActionCancelSubscriber : public URSubscriber
{
	GENERATED_BODY()

public:
	URActionCancelSubscriber();

protected:
	virtual void Init() override;

protected:
	UPROPERTY()
	URControllerComponent *ControllerComponent;
};