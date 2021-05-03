#pragma once

#include "Controller/ControllerType/JointController/RJointController.h"
#include "RSubscriber.h"
// clang-format off
#include "JointStateReplaySubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointStateReplaySubscriberParameter : public URSubscriberParameter
{
	GENERATED_BODY()

public:
	URJointStateReplaySubscriberParameter()
	{
		MessageType = TEXT("sensor_msgs/JointState");
		JointControllerName = TEXT("JointController");
	}

public:
	UPROPERTY(EditAnywhere)
	FString JointControllerName;
};

UCLASS()
class UROBOSIM_API URJointStateReplaySubscriber final : public URSubscriber
{
	GENERATED_BODY()

public:
	URJointStateReplaySubscriber();

public:
	void SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters) override;

protected:
	void CreateSubscriber() override;

public:
	UPROPERTY(EditAnywhere)
	FString JointControllerName;
};

class UROBOSIM_API FRJointStateReplaySubscriberCallback : public FROSBridgeSubscriber
{
public:
	FRJointStateReplaySubscriberCallback(const FString &InTopic, const FString &InType, UObject *InController);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	URJointController *JointController;
};