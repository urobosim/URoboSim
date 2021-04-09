#pragma once

#include "ROSCommunication/Publisher/RPublisherParameter.h"
// clang-format off
#include "FJTAServerParameter.generated.h"
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
    JointParamPath = TEXT("whole_body_controller/body/joints");
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 

  UPROPERTY(EditAnywhere)
  FString JointParamPath; 
};

UCLASS()
class UROBOSIM_API URFJTAResultPublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  URFJTAResultPublisherParameter()
  {
    MessageType = TEXT("control_msgs/FollowJointTrajectoryActionResult");
    FrameId = TEXT("odom");
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 
};