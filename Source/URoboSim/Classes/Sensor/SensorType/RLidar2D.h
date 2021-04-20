#pragma once

#include "Sensor/RSensor.h"
// clang-format off
#include "RLidar2D.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URLidar2DParameter final : public URSensorParameter
{
  GENERATED_BODY()

public:
  URLidar2DParameter()
  {
    UpdateRate = 40.f;

    ScanAngleMin = -2.27f;
    ScanAngleMax = 2.27f;
    SampleNumber = 640;

    MinimumDistance = 1.f;
    MaximumDistance = 1000.f;

    ReferenceLinkName = TEXT("base_footprint_collision");
  }

public:
  UPROPERTY(EditAnywhere)
  float UpdateRate;

  UPROPERTY(EditAnywhere)
  float ScanAngleMin;

  UPROPERTY(EditAnywhere)
  float ScanAngleMax;

  UPROPERTY(EditAnywhere)
  int SampleNumber;

  UPROPERTY(EditAnywhere)
  float MinimumDistance;

  UPROPERTY(EditAnywhere)
  float MaximumDistance;

  UPROPERTY(EditAnywhere)
  FVector LidarBodyOffset;

  UPROPERTY(EditAnywhere)
  FString ReferenceLinkName;
};

UCLASS()
class UROBOSIM_API URLidar2D final : public URSensor
{
  GENERATED_BODY()

public:
  URLidar2D();

public:
  // Called every frame
  void Tick(const float &InDeltaTime) override;

  void Init() override;

  void SetSensorParameters(URSensorParameter *&SensorParameters) override;

public:
  TArray<float> GetMeasuredData() const;

public:
  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs",
            meta = (DisplayName = "Frequency at which the sensor data is generated"))
  float UpdateRate;

  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs",
            meta = (DisplayName = "Start angle of the scan [rad]"))
  float ScanAngleMin;

  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs",
            meta = (DisplayName = "End angle of the scan [rad]"))
  float ScanAngleMax;

  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs",
            meta = (DisplayName = "Number of Samples during one revolution"))
  int SampleNumber;

  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs",
            meta = (ClampMin = "0.0", DisplayName = "Minimum Distance in meter"))
  float MinimumDistance;

  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs",
            meta = (ClampMin = "0.0", DisplayName = "Maximum Distance in meter"))
  float MaximumDistance;

  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Offset")
  FVector LidarBodyOffset;

  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs")
  FString ReferenceLinkName;

  bool bPublishResult;

  float ScanTime;

  float TimeIncrement;

  float AngularIncrement;

private:
  uint32 SCSResolution;

  TArray<float> MeasuredDistance;

  UStaticMeshComponent *ReferenceLink = nullptr;
};