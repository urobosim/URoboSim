
#pragma once

#include "RPublisher.h"
#include "Sensor/R2DLidar.h"
// clang-format off
#include "2DLidarPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API UR2DLidarPublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  UR2DLidarPublisherParameter()
  {
    Topic = TEXT("/base_scan");
    MessageType = TEXT("sensor_msgs/LaserScan");
    ROSLinkName = TEXT("base_laser_link");
  }

  UPROPERTY(EditAnywhere)
  FString LidarName;

  UPROPERTY(EditAnywhere)
  FString ROSLinkName;
};

UCLASS()
class UROBOSIM_API UR2DLidarPublisher : public URPublisher
{
  GENERATED_BODY()

public:
  UR2DLidarPublisher();

public:
  virtual void Publish() override;

  virtual void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

protected:
  virtual void Init() override;

public:
  UPROPERTY(EditAnywhere)
  FString LidarName;

  UPROPERTY(EditAnywhere)
  FString ROSLinkName;

private:
  UR2DLidar *Lidar;
};
