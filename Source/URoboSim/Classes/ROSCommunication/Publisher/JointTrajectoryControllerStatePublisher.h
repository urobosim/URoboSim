#pragma once

#include "Controller/ControllerType/JointController/RJointTrajectoryController.h"
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
    Topic = TEXT("/whole_body_controller/body/state");
    MessageType = TEXT("control_msgs/JointTrajectoryControllerState");
    FrameId = TEXT("odom");
    JointParamTopic = TEXT("/whole_body_controller/body/joints");
    JointTrajectoryControllerName = TEXT("JointTrajectoryController");
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

  UPROPERTY(EditAnywhere)
  FString JointParamTopic;

  UPROPERTY(EditAnywhere)
  FString JointTrajectoryControllerName;
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
  FString JointTrajectoryControllerName;

private:
  URJointTrajectoryController *JointTrajectoryController;

  URJointStateConfigurationClient *ConfigClient;

  UPROPERTY()
    TArray<FString> JointList;
};
