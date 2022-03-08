#pragma once

#include "Controller/ControllerType/SpecialController/RGripperController.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "PR2GCAResultPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URPR2GCAResultPublisher final : public URActionPublisher
{
  GENERATED_BODY()

public:
  URPR2GCAResultPublisher();

public:
  void Publish() override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

protected:
  void Init() override;

private:
  URGripperController *GripperController;
};
