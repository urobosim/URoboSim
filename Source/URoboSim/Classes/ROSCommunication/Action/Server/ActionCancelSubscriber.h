#pragma once

#include "RActionServer.h"
// clang-format off
#include "ActionCancelSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URActionCancelSubscriber : public URActionSubscriber
{
	GENERATED_BODY()

public:
	URActionCancelSubscriber();

public:
	virtual void CreateSubscriber() override;
};

class UROBOSIM_API FActionCancelSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FActionCancelSubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	virtual void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
	URController* Controller;
};