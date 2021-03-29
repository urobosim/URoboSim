#pragma once

#include "Controller/ControllerType/RJointController.h"
#include "Controller/RControllerComponent.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
// clang-format off
#include "FJTAGoalSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTAGoalSubscriber final : public URSubscriber
{
  GENERATED_BODY()

public:
  URFJTAGoalSubscriber();

public:
  void CreateSubscriber() override;

protected:
  void Init() override;

protected:
  UPROPERTY()
  URControllerComponent *ControllerComponent;
};

class UROBOSIM_API FRFJTAGoalSubscriberCallback final : public FROSBridgeSubscriber
{
public:
  FRFJTAGoalSubscriberCallback(const FString &InTopic, const FString &InType, UObject *InController);

  TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

  void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
  URJointController *JointController;
};