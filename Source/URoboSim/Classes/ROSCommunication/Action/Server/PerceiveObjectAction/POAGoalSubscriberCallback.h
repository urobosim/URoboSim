// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author:Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSubscriber.h"
#include "Controller/RController.h"
#include "Controller/RJointController.h"

class UROBOSIM_API FROSPerceiveObjectGoalSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FROSPerceiveObjectGoalSubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSPerceiveObjectGoalSubscriberCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URCameraController* Controller;
};
