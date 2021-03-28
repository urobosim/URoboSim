#pragma once

#include "Controller/ControllerType/RBaseController.h"
#include "Controller/RControllerComponent.h"
#include "ROSCommunication/Subscriber/RSubscriber.h"
// clang-format off
#include "RVelocityCommandSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URVelocityCommandSubscriber : public URSubscriber
{
  GENERATED_BODY()

public:
  URVelocityCommandSubscriber();

public:
  virtual void CreateSubscriber() override;

protected:
  virtual void Init() override;

protected:
  UPROPERTY()
  URControllerComponent *ControllerComponent;
};

class UROBOSIM_API FRVelocityCommandSubscriberCallback : public FROSBridgeSubscriber
{
public:
  FRVelocityCommandSubscriberCallback(const FString &InTopic, const FString &InType, UObject *InController);

  TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

  void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
  URBaseController *BaseController;
};