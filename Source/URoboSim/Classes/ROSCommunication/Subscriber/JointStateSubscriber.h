#pragma once

#include "RSubscriber.h"
// clang-format off
#include "JointStateSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointStateSubscriberParameter : public URSubscriberParameter
{
	GENERATED_BODY()

public:
	URJointStateSubscriberParameter()
	{
		MessageType = TEXT("sensor_msgs/JointState");
	}
};

UCLASS()
class UROBOSIM_API URJointStateSubscriber final : public URSubscriber
{
	GENERATED_BODY()

public:
	URJointStateSubscriber();

protected:
	void CreateSubscriber() override;
};

class UROBOSIM_API FRJointStateSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FRJointStateSubscriberCallback(const FString &InTopic, const FString &InType, ARModel *InModel);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	ARModel *Model;
};