#pragma once

#include "Controller/ControllerType/JointController/RJointController.h"
#include "ROSCommunication/Service/Client/RJointStateConfigurationClient.h"
#include "RPublisher.h"
// clang-format off
#include "JointTrajectoryControllerStatePublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointTrajectoryControllerStatePublisherParameter final : public URPublisherParameter
{
  GENERATED_BODY()

public:
  URJointTrajectoryControllerStatePublisherParameter()
  {
    MessageType = TEXT("control_msgs/FollowJointTrajectoryActionFeedback");
    FrameId = TEXT("odom");
    JointParamTopic = TEXT("/whole_body_controller/body/joints");
    JointControllerName = TEXT("JointController");
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

  UPROPERTY(EditAnywhere)
  FString JointParamTopic;

  UPROPERTY(EditAnywhere)
  FString JointControllerName;
};

UCLASS()
class UROBOSIM_API URJointTrajectoryControllerStatePublisher final : public URPublisher
{
  GENERATED_BODY()

public:
  URJointTrajectoryControllerStatePublisher();

public:
  void Publish();

  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

protected:
  void Init() override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

  UPROPERTY(EditAnywhere)
  FString JointParamTopic;

  UPROPERTY(EditAnywhere)
  FString JointControllerName;

private:
  URJointController *JointController;

  URJointStateConfigurationClient *ConfigClient;
};