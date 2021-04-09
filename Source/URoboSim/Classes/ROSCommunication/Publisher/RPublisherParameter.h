#pragma once

#include "CoreMinimal.h"
// clang-format off
#include "RPublisherParameter.generated.h"
// clang-format on

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URPublisherParameter : public UObject
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  FString Topic;

  UPROPERTY(VisibleAnywhere)
  FString MessageType;
};

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

UCLASS()
class UROBOSIM_API URActionStatusPublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  URActionStatusPublisherParameter()
  {
    MessageType = TEXT("actionlib_msgs/GoalStatusArray");
    FrameId = TEXT("odom");
  }

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 
};

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