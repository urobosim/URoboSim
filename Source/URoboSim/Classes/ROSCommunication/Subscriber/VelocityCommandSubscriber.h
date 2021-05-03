#pragma once

#include "Controller/ControllerType/BaseController/RBaseController.h"
#include "RSubscriber.h"
// clang-format off
#include "VelocityCommandSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URVelocityCommandSubscriberParameter : public URSubscriberParameter
{
  GENERATED_BODY()

public:
  URVelocityCommandSubscriberParameter()
  {
    Topic = TEXT("/base_controller/command");
    MessageType = TEXT("geometry_msgs/Twist");
    BaseControllerName = TEXT("BaseController");
  }

public:
  UPROPERTY(EditAnywhere)
  FString BaseControllerName;
};

UCLASS()
class UROBOSIM_API URVelocityCommandSubscriber final : public URSubscriber
{
  GENERATED_BODY()

public:
  URVelocityCommandSubscriber();

public:
  void SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters) override;

protected:
  void CreateSubscriber() override;

public:
  UPROPERTY(EditAnywhere)
  FString BaseControllerName;
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