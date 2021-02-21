
#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSubscriber.h"
#include "Controller/RController.h"

class UROBOSIM_API FROSTFReplaySubscriberCallback : public FROSBridgeSubscriber
{
public:
	FROSTFReplaySubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URTFController* TFController;
private:
};
