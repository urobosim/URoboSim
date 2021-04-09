#pragma once

#include "Controller/ControllerType/RBaseController.h"
#include "Controller/RControllerComponent.h"
#include "RSubscriber.h"
#include "RVelocityCommandSubscriberParameter.h"
// clang-format off
#include "RVelocityCommandSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URVelocityCommandSubscriber final : public URSubscriber
{
  GENERATED_BODY()

public:
  void CreateSubscriber() override;

protected:
  void Init() override;

protected:
  UPROPERTY()
  URControllerComponent *ControllerComponent;
};

class UROBOSIM_API FRVelocityCommandSubscriberCallback final : public FROSBridgeSubscriber
{
public:
  FRVelocityCommandSubscriberCallback(const FString &InTopic, const FString &InType, UObject *InController);

  TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

  void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
  URBaseController *BaseController;
};