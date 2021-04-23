// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author:Michael Neumann

#pragma once

#include "ROSBridgeHandler.h"
#include "ROSBridgeSubscriber.h"
#include "Controller/RController.h"

class UROBOSIM_API FROSPointHeadGoalSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FROSPointHeadGoalSubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSPointHeadGoalSubscriberCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URHeadTrajectoryController* Controller;
};
