#pragma once

#include "Controller/ControllerType/SpecialController/RCameraController.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "POAGoalSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URPOAGoalSubscriber: public URActionSubscriber
{
	GENERATED_BODY()

public:
	URPOAGoalSubscriber();

protected:
	void CreateSubscriber() override;
};

class UROBOSIM_API FPOAGoalSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FPOAGoalSubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	URCameraController* CameraController;
};