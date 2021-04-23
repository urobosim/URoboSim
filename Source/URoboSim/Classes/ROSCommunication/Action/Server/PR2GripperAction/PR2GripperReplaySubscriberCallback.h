// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author:Michael Neumann

#pragma once

#include "ROSBridgeHandler.h"
#include "ROSBridgeSubscriber.h"
#include "Controller/RController.h"

class UROBOSIM_API FROSGripperReplaySubscriberCallback : public FROSBridgeSubscriber
{
public:
	FROSGripperReplaySubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSGripperReplaySubscriberCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URGripperController* Controller;

        float Position = 0;
};
