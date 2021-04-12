#pragma once

#include "Controller/ControllerType/RJointController.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
#include "ROSCommunication/Service/Client/RGetJointsClient.h"
// clang-format off
#include "FJTAFeedbackPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTAFeedbackPublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  URFJTAFeedbackPublisherParameter()
  {
    MessageType = TEXT("control_msgs/FollowJointTrajectoryActionFeedback");
    FrameId = TEXT("odom");
    JointParamPath = TEXT("/whole_body_controller/body/joints");
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 

  UPROPERTY(EditAnywhere)
  FString JointParamPath; 
};

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
