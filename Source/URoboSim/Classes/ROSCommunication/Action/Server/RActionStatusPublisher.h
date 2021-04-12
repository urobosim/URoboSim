#pragma once

#include "RActionServer.h"
// clang-format off
#include "RActionStatusPublisher.generated.h"
// clang-format on

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

  UPROPERTY(EditAnywhere)
  FString ControllerName; 
};

UCLASS()
class UROBOSIM_API URActionStatusPublisher : public URActionPublisher
{
  GENERATED_BODY()

public:
  URActionStatusPublisher();

public:
  void Publish() override;

  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;
};
