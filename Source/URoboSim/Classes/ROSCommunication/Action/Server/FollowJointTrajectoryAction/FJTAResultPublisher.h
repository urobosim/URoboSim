#pragma once

#include "Controller/ControllerType/RJointController.h"
#include "ROSCommunication/Publisher/RPublisher.h"
// clang-format off
#include "FJTAResultPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTAResultPublisher : public URPublisher
{
  GENERATED_BODY()

public:
  URFJTAResultPublisher();

public:
  virtual void Publish() override;

protected:
  virtual void Init() override;

private:
  URJointController *JointController;

  UPROPERTY(EditAnywhere)
  FString FrameId;
};
