#pragma once

#include "Controller/ControllerType/RJointController.h"
#include "FJTAServerParameter.h"
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
  FString JointControllerName;

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
