#pragma once

#include "Controller/ControllerType/RJointController.h"
#include "ROSCommunication/Action/Server/RActionServer.h"
// clang-format off
#include "FJTAResultPublisher.generated.h"
// clang-format on

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

UCLASS()
class UROBOSIM_API URFJTAResultPublisher final : public URActionPublisher
{
  GENERATED_BODY()

public:
  URFJTAResultPublisher();

public:
  void Publish() override;

protected:
  void Init() override;

public:
  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId; 

private:
  URJointController *JointController;
};
