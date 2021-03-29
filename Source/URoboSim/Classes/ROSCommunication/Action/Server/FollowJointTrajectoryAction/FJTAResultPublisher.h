#pragma once

#include "Controller/ControllerType/RJointController.h"
#include "ROSCommunication/Publisher/RPublisher.h"
// clang-format off
#include "FJTAResultPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTAResultPublisher final : public URPublisher
{
  GENERATED_BODY()

public:
  URFJTAResultPublisher();

public:
  void Publish() override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

protected:
  void Init() override;

private:
  URJointController *JointController;
};
