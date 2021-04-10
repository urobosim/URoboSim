#pragma once

#include "Controller/RControllerComponent.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
#include "RActionServerParameter.h"
// clang-format off
#include "RActionCancelSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URActionCancelSubscriber : public URSubscriber
{
	GENERATED_BODY()

public:
	URActionCancelSubscriber();

public:
  UPROPERTY(EditAnywhere)
  FString ControllerName; 

protected:
	virtual void Init() override;

protected:
  URController *Controller;
};