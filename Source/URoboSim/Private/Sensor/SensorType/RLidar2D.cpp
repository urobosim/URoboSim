#include "Sensor/SensorType/RLidar2D.h"

DEFINE_LOG_CATEGORY_STATIC(LogRLidar2D, Log, All);

URLidar2D::URLidar2D()
{
  SCSResolution = 360;

  ScanAngleMin = -2.27f;
  ScanAngleMax = 2.27f;
  SampleNumber = 640;

  UpdateRate = 40.f;
}

void URLidar2D::Init()
{
  Super::Init();
  ScanTime = 1.f / UpdateRate;
  TimeIncrement = ScanTime / SampleNumber;
  AngularIncrement = FMath::Abs((ScanAngleMax - ScanAngleMin) / SampleNumber);
  SCSResolution = FMath::CeilToInt(360.f / FMath::RadiansToDegrees(AngularIncrement));
  MeasuredDistance.AddZeroed(SCSResolution);
  TArray<UStaticMeshComponent *> ActorComponents;

  if (GetOwner())
  {
    GetOwner()->GetComponents(ActorComponents);

    for (UStaticMeshComponent *&ActorComponent : ActorComponents)
    {
      if (ActorComponent->GetName().Equals(ReferenceLinkName))
      {
        ReferenceLink = ActorComponent;
        break;
      }
    }
    if (!ReferenceLink)
    {
      UE_LOG(LogRLidar2D, Error, TEXT("%s of %s not found"), *ReferenceLinkName, *GetOwner()->GetName());
    }
  }
}

void URLidar2D::SetSensorParameters(URSensorParameter *&SensorParameters)
{
  URLidar2DParameter *Lidar2DParameters = Cast<URLidar2DParameter>(SensorParameters);
  if (Lidar2DParameters)
  {
    UpdateRate = Lidar2DParameters->UpdateRate;
    ScanAngleMin = Lidar2DParameters->ScanAngleMin;
    ScanAngleMax = Lidar2DParameters->ScanAngleMax;
    SampleNumber = Lidar2DParameters->SampleNumber;
    MinimumDistance = Lidar2DParameters->MinimumDistance;
    MaximumDistance = Lidar2DParameters->MaximumDistance;
    LidarBodyOffset = Lidar2DParameters->LidarBodyOffset;
    ReferenceLinkName = Lidar2DParameters->ReferenceLinkName;
  }
}

void URLidar2D::Tick(const float &InDeltaTime)
{
  static float PassedTime = 0.f;
  PassedTime += InDeltaTime;
  if (PassedTime < ScanTime)
  {
    return;
  }
  else
  {
    PassedTime = 0.f;
    FVector LidarBodyLocation;
    FRotator LidarBodyRotation;
    if (ReferenceLink)
    {
      LidarBodyLocation = ReferenceLink->GetComponentLocation() + LidarBodyOffset;
      LidarBodyRotation = ReferenceLink->GetComponentRotation();
    }

    for (uint32 i = 0; i < SCSResolution; i++)
    {
      float Angle = i * FMath::RadiansToDegrees(AngularIncrement);
      FRotator ResultRot(0, 180 - Angle, 0);
      FVector EndTrace = MaximumDistance * LidarBodyRotation.RotateVector(ResultRot.Vector()) + LidarBodyLocation;
      FCollisionQueryParams TraceParams = FCollisionQueryParams(TEXT("TraceLaser"), true, GetOwner());
      TraceParams.bTraceComplex = true;
      TraceParams.bReturnPhysicalMaterial = false;

      FHitResult HitInfo(EForceInit::ForceInit);
      GetWorld()->LineTraceSingleByChannel(
          HitInfo,
          LidarBodyLocation,
          EndTrace,
          ECollisionChannel::ECC_MAX,
          TraceParams,
          FCollisionResponseParams::DefaultResponseParam);
      MeasuredDistance[i] = HitInfo.Distance / 100.;
    }
    bPublishResult = true;
  }
}

TArray<float> URLidar2D::GetMeasuredData() const
{
  TArray<float> MeasuredData;
  int Index = SCSResolution / 2 + ScanAngleMin / AngularIncrement;
  MeasuredData.Append(&MeasuredDistance[Index], SampleNumber);

  return MeasuredData;
}