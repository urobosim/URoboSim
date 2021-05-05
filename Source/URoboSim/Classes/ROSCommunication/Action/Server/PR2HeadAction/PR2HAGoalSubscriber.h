#pragma once

#include "Controller/ControllerType/SpecialController/RHeadController.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "PR2HAGoalSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URPR2HAGoalSubscriber: public URActionSubscriber
{
	GENERATED_BODY()

public:
	URPR2HAGoalSubscriber();

public:
	virtual void CreateSubscriber();
};

class UROBOSIM_API FRPR2HAGoalSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FRPR2HAGoalSubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	URHeadTrajectoryController* HeadController;
};
