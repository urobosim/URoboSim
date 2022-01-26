#pragma once

#include "Controller/ControllerType/JointController/RJointController.h"
#include "RSubscriber.h"
// clang-format off
#include "GripperCommandSubscriber.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URGripperCommandSubscriberParameter : public URSubscriberParameter
{
  GENERATED_BODY()

public:
  URGripperCommandSubscriberParameter()
  {
    Topic = TEXT("/gripper_controller/command");
    MessageType = TEXT("std_msgs/Float32");
    JointControllerName = TEXT("JointController");
  }

public:
  UPROPERTY(EditAnywhere)
  FString JointControllerName;

  UPROPERTY(EditAnywhere)
  FString GripperJointLeftName;

  UPROPERTY(EditAnywhere)
  FString GripperJointRightName;

  UPROPERTY(EditAnywhere)
  TSoftObjectPtr<AActor> Object;
};

UCLASS()
class UROBOSIM_API URGripperCommandSubscriber final : public URSubscriber
{
  GENERATED_BODY()

public:
  URGripperCommandSubscriber();

public:
  void SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters) override;

protected:
  void CreateSubscriber() override;

public:
  UPROPERTY(EditAnywhere)
  FString JointControllerName;

  UPROPERTY(EditAnywhere)
  FString GripperJointLeftName;

  UPROPERTY(EditAnywhere)
  FString GripperJointRightName;

  UPROPERTY(EditAnywhere)
  TSoftObjectPtr<AActor> Object;
};

class UROBOSIM_API FRGripperCommandSubscriberCallback final : public FROSBridgeSubscriber
{
public:
  FRGripperCommandSubscriberCallback(const FString &InTopic, const FString &InType, UObject *InController, FString GripperJointLeftName, FString GripperJointRightName, TSoftObjectPtr<AActor> Object);

  TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

  void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

private:
  URJointController *JointController;

  FString GripperJointLeftName;

  FString GripperJointRightName;

  TSoftObjectPtr<AActor> Object;
};