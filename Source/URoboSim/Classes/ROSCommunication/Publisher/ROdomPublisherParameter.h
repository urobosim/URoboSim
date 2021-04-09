#pragma once

#include "RPublisherParameter.h"
// clang-format off
#include "ROdomPublisherParameter.generated.h"
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