#pragma once

#include "ROSCommunication/Service/Client/RJointStateConfigurationClient.h"
#include "RPublisher.h"
// clang-format off
#include "JointStatePublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointStatePublisherParameter final : public URPublisherParameter
{
  GENERATED_BODY()

public:
  URJointStatePublisherParameter()
  {
    Topic = TEXT("/joint_states");
    MessageType = TEXT("sensor_msgs/JointState");
    JointParamTopic = TEXT("/hardware_interface/joints");
  }

  UPROPERTY(EditAnywhere)
  FString JointParamTopic;

};

UCLASS()
class UROBOSIM_API URJointStatePublisher final : public URPublisher
{
  GENERATED_BODY()

public:
  URJointStatePublisher();

public:
  void Publish() override;

  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

protected:
  void Init() override;

public:
  UPROPERTY(EditAnywhere)
  FString JointParamTopic;

  UPROPERTY(EditAnywhere)
  FString FrameId;

private:
  URJointStateConfigurationClient *ConfigClient;

  TArray<FString> ListJointName;

  TArray<double> ListJointPosition, ListJointVelocity, ListJointEffort;
};
