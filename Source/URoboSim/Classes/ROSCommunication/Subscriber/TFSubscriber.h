#pragma once

#include "Controller/RController.h"
#include "RSubscriber.h"
// clang-format off
#include "TFSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URTFSubscriberParameter : public URSubscriberParameter
{
	GENERATED_BODY()

public:
	URTFSubscriberParameter()
	{
		MessageType = TEXT("tf2_msgs/TFMessage");
		TFControllerName = TEXT("TFController");
	}

public:
	UPROPERTY(EditAnywhere)
	FString TFControllerName;
};

UCLASS()
class UROBOSIM_API URTFSubscriber final : public URSubscriber
{
	GENERATED_BODY()

public:
	URTFSubscriber();

public:
	void SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters) override;

protected:
	void CreateSubscriber() override;

public:
	UPROPERTY(EditAnywhere)
	FString TFControllerName;
};

class UROBOSIM_API FRTFSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FRTFSubscriberCallback(const FString &InTopic, const FString &InType, UObject *InController);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	URTFController *TFController;
};
