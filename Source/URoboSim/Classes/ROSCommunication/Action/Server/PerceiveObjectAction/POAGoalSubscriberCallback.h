// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author:Michael Neumann

#pragma once

#include "ROSBridgeHandler.h"
#include "ROSBridgeSubscriber.h"
#include "Controller/ControllerType/JointController/RJointController.h"

class UROBOSIM_API FROSPerceiveObjectGoalSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FROSPerceiveObjectGoalSubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSPerceiveObjectGoalSubscriberCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URCameraController* Controller;
};
