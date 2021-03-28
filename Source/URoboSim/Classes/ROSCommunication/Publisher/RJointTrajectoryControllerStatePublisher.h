#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "Controller/ControllerType/RJointController.h"
// clang-format off
#include "RJointTrajectoryControllerStatePublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointTrajectoryControllerStatePublisher : public URPublisher
{
  GENERATED_BODY()

public:
  URJointTrajectoryControllerStatePublisher();

  virtual void Publish() override;

protected:
  virtual void Init() override;

private:
  URJointController *JointController;

  UPROPERTY(EditAnywhere)
  FString FrameId;
};
