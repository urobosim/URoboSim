// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author:Michael Neumann

#pragma once

#include "ROSBridgeHandler.h"
#include "ROSBridgeSubscriber.h"
#include "Controller/RController.h"

class UROBOSIM_API FROSGripperCommandSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FROSGripperCommandSubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSGripperCommandSubscriberCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URGripperController* Controller;
};
