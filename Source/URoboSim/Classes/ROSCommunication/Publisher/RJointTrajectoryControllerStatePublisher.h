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
  void Publish() override;

public:
  const URJointTrajectoryControllerStatePublisherParameter *GetJointTrajectoryControllerStatePublisherParameters() const { return Cast<URJointTrajectoryControllerStatePublisherParameter>(PublisherParameters); }

protected:
  void Init() override;

private:
  URJointController *JointController;
};
