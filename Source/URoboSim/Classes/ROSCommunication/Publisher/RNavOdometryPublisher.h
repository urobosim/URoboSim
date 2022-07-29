#pragma once

#include "RPublisher.h"
#include "RNavOdometryPublisher.generated.h"

UCLASS()
class UROBOSIM_API URNavOdometryPublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  URNavOdometryPublisherParameter()
  {
    Topic = TEXT("/base_odometry/odom");
    MessageType = TEXT("nav_msgs/Odometry");
    OdomFrameId = TEXT("odom_combined");
    BaseFrameId = TEXT("base_footprint");
  }

public:
  UPROPERTY(EditAnywhere)
  FString OdomFrameId;

  UPROPERTY(EditAnywhere)
  FString BaseFrameId;
};

UCLASS()
class UROBOSIM_API URNavOdometryPublisher : public URPublisher
{
  GENERATED_BODY()

public:
  URNavOdometryPublisher();

  UPROPERTY(EditAnywhere)
  FString OdomFrameId;

  UPROPERTY(EditAnywhere)
  FString BaseFrameId;
public:
  virtual void Publish() override;

  virtual void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

public:
};
