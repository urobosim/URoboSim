#pragma once

#include "RPublisher.h"
#include "RJointTrajectoryControllerStatePublisherParameter.h"
#include "Controller/ControllerType/RJointController.h"
// clang-format off
#include "RJointTrajectoryControllerStatePublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointTrajectoryControllerStatePublisher final : public URPublisher
{
  GENERATED_BODY()

public:
  URJointTrajectoryControllerStatePublisher();

public:
  void Publish() override;

protected:
  void Init() override;

public:
  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

  UPROPERTY(EditAnywhere)
  FString JointControllerName;

private:
  URJointController *JointController;
};
