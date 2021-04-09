#pragma once

#include "RPublisherParameter.h"
// clang-format off
#include "RJointTrajectoryControllerStatePublisherParameter.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointTrajectoryControllerStatePublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  URJointTrajectoryControllerStatePublisherParameter()
  {
    Topic = TEXT("/whole_body_controller/body/state");
    MessageType = TEXT("control_msgs/JointTrajectoryControllerState");
    FrameId = TEXT("odom");
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;
};
