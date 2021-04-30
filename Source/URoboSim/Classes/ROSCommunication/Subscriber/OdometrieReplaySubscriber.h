#pragma once

#include "Controller/ControllerType/BaseController/RBaseController.h"
#include "RSubscriber.h"
// clang-format off
#include "OdometrieReplaySubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API UROdometrieReplaySubscriberParameter : public URSubscriberParameter
{
	GENERATED_BODY()

public:
	UROdometrieReplaySubscriberParameter()
	{
		MessageType = TEXT("tf2_msgs/TFMessage");
		BaseControllerName = TEXT("BaseController");
	}

public:
	UPROPERTY(EditAnywhere)
	FString BaseControllerName;
};

UCLASS()
class UROBOSIM_API UROdometrieReplaySubscriber final : public URSubscriber
{
	GENERATED_BODY()

public:
	UROdometrieReplaySubscriber();

public:
	void SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters) override;

protected:
	void CreateSubscriber() override;
	
public:
	UPROPERTY(EditAnywhere)
	FString BaseControllerName;
};

class UROBOSIM_API FROdometrieReplaySubscriberCallback : public FROSBridgeSubscriber
{
public:
	FROdometrieReplaySubscriberCallback(const FString &InTopic, const FString &InType, UObject *InController);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	URBaseController *BaseController;
};
