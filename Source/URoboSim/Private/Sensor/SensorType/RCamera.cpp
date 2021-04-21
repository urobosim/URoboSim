#include "Sensor/SensorType/RCamera.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogRCamera, Log, All);

URCamera::URCamera()
{
  CameraName = TEXT("RGBDCamera");
  ReferenceLinkName = TEXT("");
  CameraPoseOffset.SetRotation(FQuat(FRotator(90.f, 0.f, 90.f)));
}

void URCamera::Init()
{
  Super::Init();

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
    if (ReferenceLink)
    {
      TArray<AActor *> Actors;
      UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARGBDCamera::StaticClass(), Actors);
      for (AActor *&Actor : Actors)
      {
        UE_LOG(LogRCamera, Log, TEXT("Found Camera %s"), *Actor->GetName())
        if (Actor->GetName().Equals(CameraName))
        {
          Actor->AttachToComponent(ReferenceLink, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
          Actor->AddActorLocalOffset(CameraPoseOffset.GetLocation());
          Actor->AddActorLocalRotation(CameraPoseOffset.GetRotation());
          return;
        }
      }
      UE_LOG(LogRCamera, Error, TEXT("%s of %s not found"), *CameraName, *GetOwner()->GetName())
    }
    else
    {
      UE_LOG(LogRCamera, Error, TEXT("%s of %s not found"), *ReferenceLinkName, *GetOwner()->GetName());
    }
  }
  else
  {
    UE_LOG(LogRCamera, Error, TEXT("Owner of %s not found"), *GetName());
  }
}

void URCamera::SetSensorParameters(URSensorParameter *&SensorParameters)
{
  URCameraParameter *CameraParameters = Cast<URCameraParameter>(SensorParameters);
  if (CameraParameters)
  {
    Super::SetSensorParameters(SensorParameters);
    CameraName = CameraParameters->CameraName;
    ReferenceLinkName = CameraParameters->ReferenceLinkName;
    CameraPoseOffset = CameraParameters->CameraPoseOffset;
  }
}

void URCamera::Tick(const float &InDeltaTime)
{
}