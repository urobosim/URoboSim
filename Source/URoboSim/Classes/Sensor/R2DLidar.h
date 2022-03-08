#pragma once

#include "Sensor/RSensor.h"
#include "R2DLidar.generated.h"

UCLASS(ClassGroup=(RobotSensor), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API UR2DLidar : public URSensor
{
  GENERATED_BODY()

public:
    UR2DLidar();

  virtual void BeginPlay() override;
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  virtual void Init() override;
  virtual TArray<float> GetMeasurementData();
  // virtual void Tick(float InDeltaTime) override;


  UPROPERTY(EditAnywhere, Category = "LiDAR|Debugging")
    bool bShowDebugLog;

  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs",
            meta = ( DisplayName = "frequency at which the sensor data is generated"))
    float UpdateRate;

  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs",
            meta = ( DisplayName = "start angle of the scan [rad]"))
    float ScanAngleMin;

  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs",
            meta = ( DisplayName = "end angle of the scan [rad]"))
    float ScanAngleMax;

  /**
   * The angular resolution of the laser scanner in radians
   */


  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs",
            meta = (DisplayName = "Number of Samples during one revolution"))
    float SampleNumber;

  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs",
            meta = (DisplayName = "This number multiplied by SampleNumber equals the generated data points"))
    float Resolution;

  /**
   * Minimum distance that an object has to be away from the sensor to get realized
   */
  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs",
            meta = (ClampMin = "0.0", DisplayName="Minimum Distance in meter"))
    float MinimumDistance;

  /**
   * Maximum distance that an object can be away from the sensor and still be realized
   */
  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Offset")
    FVector Offset;

  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs",
              meta = (ClampMin = "0.0", DisplayName = "Maximum Distance in meter"))
    float MaximumDistance;

    /**
   * Maximum distance that an object can be away from the sensor and still be realized
   */
    UPROPERTY()
    USceneComponent* Sensor;

  UPROPERTY()
    bool bPublishResult;

  UPROPERTY()
    float TimeIncrement;

  UPROPERTY()
    float ScanTime;

  UPROPERTY()
    float AngularIncrement;

  UPROPERTY(EditAnywhere, Category = "LiDAR|Scanning Specs")
  FString LidarRef;
 private:

  UPROPERTY()
  uint32 SCSResolution;


  UPROPERTY()
    USceneCaptureComponent2D* DepthCaptureFront;
  UPROPERTY()
    USceneCaptureComponent2D* DepthCaptureLeft;
  UPROPERTY()
    USceneCaptureComponent2D* DepthCaptureRight;
  UPROPERTY()
    USceneCaptureComponent2D* DepthCaptureBack;

  TArray<FFloat16Color> DepthImage;

  UPROPERTY()
  float TimePassed;


  UPROPERTY()
  TArray<float> DistanceMeasurement;

  UPROPERTY()
  UStaticMeshComponent* ReferenceLink = nullptr;
};
