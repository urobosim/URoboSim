#pragma once

#include "Controller/ControllerType/RJointController.h"
#include "FJTAServerParameter.h"
#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Service/Client/RGetJointsClient.h"
// clang-format off
#include "FJTAFeedbackPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTAFeedbackPublisher final : public URPublisher
{
  GENERATED_BODY()

public:
  URFJTAFeedbackPublisher();

public:
  void Publish() override;

protected:
  void Init() override;

public:
  const URFJTAFeedbackPublisherParameter *GetFJTAFeedbackPublisherParameters() const { return Cast<URFJTAFeedbackPublisherParameter>(PublisherParameters); }

private:
  URJointController *JointController;

  URGetJointsClient *GetJointsClient;
};
