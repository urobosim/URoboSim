#pragma once

#include "Controller/ControllerType/RJointController.h"
#include "ROSCommunication/Action/Server/RActionCancelSubscriber.h"
// clang-format off
#include "FJTACancelSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTACancelSubscriber : public URActionCancelSubscriber
{
	GENERATED_BODY()

public:
	virtual void CreateSubscriber() override;
};

class UROBOSIM_API FRFJTACancelSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FRFJTACancelSubscriberCallback(const FString &InTopic, const FString &InType, UObject *InController);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	URJointController *JointController;
};