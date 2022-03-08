#pragma once

#include "Controller/ControllerType/SpecialController/RGripperController.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "PR2GCAGoalSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URPR2GCAGoalSubscriber : public URActionSubscriber
{
	GENERATED_BODY()

public:
	URPR2GCAGoalSubscriber();

public:
	void CreateSubscriber() override;
};

class UROBOSIM_API FRPR2GCAGoalSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FRPR2GCAGoalSubscriberCallback(const FString &InTopic, const FString &InType, UObject *InController);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	URGripperController *GripperController;
};