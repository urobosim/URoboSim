#pragma once

#include "RPublisher.h"
#include "Sensor/SensorType/RLidar2D.h"
// clang-format off
#include "RLidar2DPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URLidar2DPublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  URLidar2DPublisherParameter()
  {
    Topic = TEXT("/base_scan");
    MessageType = TEXT("sensor_msgs/LaserScan");
    LidarReferenceROSLinkName = TEXT("base_laser_link");
    LidarName = TEXT("Laser");
  }

public:
  UPROPERTY(EditAnywhere)
  FString LidarReferenceROSLinkName;

  UPROPERTY(EditAnywhere)
  FString LidarName;
};

UCLASS()
class UROBOSIM_API URLidar2DPublisher : public URPublisher
{
  GENERATED_BODY()

public:
  URLidar2DPublisher();

public:
  void Publish() override;

  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

protected:
  void Init() override;

private:
  void SetLidar();

private:
  UPROPERTY(EditAnywhere)
  FString LidarReferenceROSLinkName;

  UPROPERTY(EditAnywhere)
  FString LidarName;

  URLidar2D *Lidar;
};
