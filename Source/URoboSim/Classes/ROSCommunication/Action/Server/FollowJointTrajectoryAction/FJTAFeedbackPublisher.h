#pragma once

#include "Controller/ControllerType/RJointController.h"
#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Service/Client/RGetJointsClient.h"
// clang-format off
#include "FJTAFeedbackPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTAFeedbackPublisher : public URPublisher
{
  GENERATED_BODY()

public:
  URFJTAFeedbackPublisher();

  virtual void Publish() override;

protected:
  virtual void Init() override;

private:
  URJointController *JointController;

  URGetJointsClient *GetJointsClient;

  UPROPERTY(EditAnywhere)
  FString JointParamPath;

  UPROPERTY(EditAnywhere)
  FString FrameId;
};
