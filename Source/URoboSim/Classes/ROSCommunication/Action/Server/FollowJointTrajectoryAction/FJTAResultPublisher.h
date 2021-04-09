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
  void Publish() override;

protected:
  void Init() override;

public:
  const URFJTAResultPublisherParameter *GetFJTAResultPublisherParameters() const { return Cast<URFJTAResultPublisherParameter>(PublisherParameters); }

private:
  URJointController *JointController;
};
