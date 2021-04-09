#pragma once

#include "RPublisherParameter.h"
// clang-format off
#include "RJointStatePublisherParameter.generated.h"
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