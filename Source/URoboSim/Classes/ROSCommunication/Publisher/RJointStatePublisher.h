#pragma once

#include "RPublisher.h"
// clang-format off
#include "RJointStatePublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointStatePublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  URJointStatePublisherParameter()
  {
    Topic = TEXT("/body/joint_states");
    MessageType = TEXT("sensor_msgs/JointState");
    FrameId = TEXT("odom");
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 
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
  TMap<FString, FJointState> JointStates;

private:
  UPROPERTY(EditAnywhere)
  FString FrameId; 
};