#pragma once

#include "Controller/RController.h"
#include "Controller/ControllerType/SpecialController/RTFController.h"
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

// UCLASS()
// class UROBOSIM_API UTFHander final : public UObject
// {
//   GENERATED_BODY()

// public:


// protected:
//   UPROPERTY(EditAnywhere)
//     FString FixedFrame;

//   UPROPERTY(EditAnywhere)
//     UTFController* TFController;

// };

UCLASS()
class UROBOSIM_API UTFSubscriber final : public USubscriber
{
	GENERATED_BODY()

public:
	UTFSubscriber();

        void SetController(UObject* InController);
protected:

	void CreateSubscriber() override;

private:
	UTFController *TFController;
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
	UTFController *TFController;
};
