#pragma once

#include "RPublisher.h"
#include "Controller/ControllerType/RJointController.h"
// clang-format off
#include "RJointTrajectoryControllerStatePublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointTrajectoryControllerStatePublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  URJointTrajectoryControllerStatePublisherParameter()
  {
    Topic = TEXT("/whole_body_controller/body/state");
    MessageType = TEXT("control_msgs/JointTrajectoryControllerState");
    FrameId = TEXT("odom");
    JointControllerName = TEXT("JointController");
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

  UPROPERTY(EditAnywhere)
  FString JointControllerName;
};

UCLASS()
class UROBOSIM_API URJointTrajectoryControllerStatePublisher final : public URPublisher
{
  GENERATED_BODY()

public:
  URJointTrajectoryControllerStatePublisher();

public:
  void Publish() override;

  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

protected:
  void Init() override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

  UPROPERTY(EditAnywhere)
  FString JointControllerName;

private:
  URJointController *JointController;
};
