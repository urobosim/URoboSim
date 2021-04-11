#pragma once

#include "Controller/ControllerType/RJointController.h"
#include "FJTAServer.h"
// clang-format off
#include "FJTAResultPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTAResultPublisher final : public URActionPublisher
{
  GENERATED_BODY()

public:
  URFJTAResultPublisher();

public:
  void Publish() override;

protected:
  void Init() override;

public:
  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 

private:
  URJointController *JointController;
};
