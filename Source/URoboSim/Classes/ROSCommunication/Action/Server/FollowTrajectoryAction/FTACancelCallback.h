
#pragma once

#include "ROSBridgeHandler.h"
#include "ROSBridgeSubscriber.h"
#include "Controller/ControllerType/JointController/RJointController.h"

class UROBOSIM_API FROSFollowTrajectoryActionCancelCallback : public FROSBridgeSubscriber
{
public:
	FROSFollowTrajectoryActionCancelCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSFollowTrajectoryActionCancelCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URJointController* JointController;
private:
};
