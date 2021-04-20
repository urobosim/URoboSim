#pragma once

#include "RPublisher.h"
// clang-format off
#include "ROdomPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API UROdomPublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  UROdomPublisherParameter()
  {
    Topic = TEXT("/base/joint_states");
    MessageType = TEXT("sensor_msgs/JointState");
    FrameId = TEXT("odom");
    FrameNames.Add(TEXT("odom_x_joint"));
    FrameNames.Add(TEXT("odom_y_joint"));
    FrameNames.Add(TEXT("odom_z_joint"));
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

  UPROPERTY(EditAnywhere)
	TArray<FString> FrameNames;

  UPROPERTY(EditAnywhere)
  bool bProjectToGround = true;
};

UCLASS()
class UROBOSIM_API UROdomPublisher final : public URPublisher
{
  GENERATED_BODY()

public:
  UROdomPublisher();

public:
  void Publish() override;

  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

protected:
  void Init() override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

  UPROPERTY(EditAnywhere)
	TArray<FString> FrameNames;

private:
	void CalculateOdomStates();

private:
  UPROPERTY(EditAnywhere)
  bool bProjectToGround = true;

	TArray<double> OdomPosition;

	TArray<double> OdomVelocity;
};
