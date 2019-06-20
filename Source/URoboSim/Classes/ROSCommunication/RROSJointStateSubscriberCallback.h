// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author:Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSubscriber.h"
#include "Physics/RModel.h"
#include "Physics/RJoint.h"

/**
 *
 */
class UROBOSIM_API FROSJointStateSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FROSJointStateSubscriberCallback(const FString& InTopic, const FString& InType, ARModel* InModel);

	~FROSJointStateSubscriberCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	ARModel* Model;
private:
};
