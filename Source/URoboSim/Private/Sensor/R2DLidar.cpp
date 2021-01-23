#include "Sensor/R2DLidar.h"
#include "Algo/Reverse.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"


UR2DLidar::UR2DLidar()
{
  PrimaryComponentTick.bCanEverTick = true;

  // SCSResolution = 360;
  ScanAngleMin = -2.27;
  ScanAngleMax = 2.27;
  SampleNumber = 640;

  UpdateRate = 40.;

}

void UR2DLidar::BeginPlay()
{
  Super::BeginPlay();
  Init();
}

void UR2DLidar::Init()
{
  ScanTime = 1.0/ UpdateRate;
  TimeIncrement = ScanTime / SampleNumber;
  AngularIncrement = FMath::Abs((ScanAngleMax - ScanAngleMin) / SampleNumber);
  SCSResolution = FMath::CeilToInt(360./ FMath::RadiansToDegrees(AngularIncrement));
  DistanceMeasurement.AddZeroed(SCSResolution);
  TArray<URStaticMeshComponent*> ActorComponents;
  GetOwner()->GetComponents(ActorComponents);

  for (auto& Component : ActorComponents)
  {
    if (Component->GetName().Equals(LidarRef))
    {
      ReferenceLink = Component;
    }
  }
  if (!ReferenceLink)
  {
    UE_LOG(LogTemp, Error, TEXT("LidarRef not found"));
    return;
  }
}

void UR2DLidar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  TimePassed += DeltaTime;
  if(TimePassed < ScanTime)
    {
      return;
    }
  TimePassed = 0;
  FVector LidarBodyLoc;
  FRotator LidarBodyRot;
  if(ReferenceLink)
    {
      LidarBodyLoc = ReferenceLink->GetComponentLocation() + Offset;
      LidarBodyRot = ReferenceLink->GetComponentRotation();
    }

  for(int i = 0; i < (int) SCSResolution; i++)
    {
      float Angle = i * FMath::RadiansToDegrees(AngularIncrement);
      FRotator ResultRot(0, 180 - Angle, 0);
      FVector EndTrace = MaximumDistance * LidarBodyRot.RotateVector(ResultRot.Vector()) + LidarBodyLoc;
      FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("Laser_Trace")), true, GetOwner());
      TraceParams.bTraceComplex = true;
      TraceParams.bReturnPhysicalMaterial = false;

      FHitResult HitInfo(ForceInit);
      GetWorld()->LineTraceSingleByChannel(
                                           HitInfo,
                                           LidarBodyLoc,
                                           EndTrace,
                                           ECC_MAX,
                                           TraceParams,
                                           FCollisionResponseParams::DefaultResponseParam
                                           );
      DistanceMeasurement[i] = HitInfo.Distance/ 100.;
    }
  bPublishResult = true;
}

TArray<float> UR2DLidar::GetMeasurementData()
{
  TArray<float> MeasurmentsToPublish;
  int Index = 0;
  Index = SCSResolution / 2  + ScanAngleMin / AngularIncrement;
  MeasurmentsToPublish.Append(&DistanceMeasurement[Index], SampleNumber);
  return MeasurmentsToPublish;
}
