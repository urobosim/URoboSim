#pragma once

#include "Controller/ControllerType/SpecialController/RHeadController.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "PR2HAResultPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URPR2HAResultPublisher final : public URActionPublisher
{
  GENERATED_BODY()

public:
  URPR2HAResultPublisher();

public:
  void Publish() override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

protected:
  void Init() override;

private:
  URHeadTrajectoryController *HeadController;
};
