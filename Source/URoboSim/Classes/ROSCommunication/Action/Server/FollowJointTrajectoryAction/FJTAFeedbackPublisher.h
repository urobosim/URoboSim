#pragma once

#include "Controller/ControllerType/RJointController.h"
#include "FJTAServer.h"
#include "ROSCommunication/Service/Client/RGetJointsClient.h"
// clang-format off
#include "FJTAFeedbackPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTAFeedbackPublisher final : public URActionPublisher
{
  GENERATED_BODY()

public:
  URFJTAFeedbackPublisher();

public:
  void Publish() override;

protected:
  void Init() override;

public:
  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 

  UPROPERTY(EditAnywhere)
  FString JointParamPath; 

private:
  URJointController *JointController;

  URGetJointsClient *GetJointsClient;
};
