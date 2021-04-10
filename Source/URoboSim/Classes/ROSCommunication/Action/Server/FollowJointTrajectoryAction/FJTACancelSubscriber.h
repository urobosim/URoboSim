#pragma once

#include "Controller/ControllerType/RJointController.h"
#include "ROSCommunication/Action/Server/RActionCancelSubscriber.h"
// clang-format off
#include "FJTACancelSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTACancelSubscriber final : public URActionCancelSubscriber
{
	GENERATED_BODY()

public:
	URFJTACancelSubscriber();

public:
	void CreateSubscriber() override;

};

class UROBOSIM_API FRFJTACancelSubscriberCallback final : public FROSBridgeSubscriber
{
public:
	FRFJTACancelSubscriberCallback(const FString &InTopic, const FString &InType, UObject *InController);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	URJointController *JointController;
};