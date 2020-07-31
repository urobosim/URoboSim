// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author:Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSubscriber.h"
#include "Controller/RController.h"

class UROBOSIM_API FROSVelocityCommandSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FROSVelocityCommandSubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSVelocityCommandSubscriberCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URBaseController* BaseController;
private:
};
