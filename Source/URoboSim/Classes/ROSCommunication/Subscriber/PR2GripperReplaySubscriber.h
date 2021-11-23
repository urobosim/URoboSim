#pragma once

#include "Controller/ControllerType/SpecialController/RGripperController.h"
#include "RSubscriber.h"
// clang-format off
#include "PR2GripperReplaySubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URGripperReplayParameter : public URSubscriberParameter
{
	GENERATED_BODY()

public:
	URGripperReplayParameter()
	{
		MessageType = TEXT("sensor_msgs/JointState");
		ControllerName = TEXT("GripperController");
	}

public:
	UPROPERTY(EditAnywhere)
	FString ControllerName;
};

UCLASS()
class UROBOSIM_API URPR2GripperReplaySubscriber final : public URSubscriber
{
	GENERATED_BODY()

public:
	URPR2GripperReplaySubscriber();

protected:
	void CreateSubscriber() override;

};

class UROBOSIM_API FRPR2GripperReplaySubscriberCallback final : public FROSBridgeSubscriber
{
public:
	FRPR2GripperReplaySubscriberCallback(const FString &InTopic, const FString &InType, UObject *InController);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	URGripperController *GripperController;

	float Position = 0;
};
