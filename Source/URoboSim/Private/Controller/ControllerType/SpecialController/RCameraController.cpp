#include "Controller/ControllerType/SpecialController/RCameraController.h"

URCameraController::URCameraController()
{

}

void URCameraController::SetControllerParameters(URControllerParameter *&ControllerParameters)
{
  URCameraControllerParameter *CameraControllerParameters = Cast<URCameraControllerParameter>(ControllerParameters);
  if (CameraControllerParameters)
  {
    CameraRef = CameraControllerParameters->CameraRef;
    CameraName = CameraControllerParameters->CameraName;
    PoseOffset = CameraControllerParameters->PoseOffset;
  }
}

void URCameraController::Init()
{
  Super::Init();

  if (!GetOwner())
  {
    UE_LOG(LogTemp, Error, TEXT("%s is not attached to ARModel"), *GetName());
    return;
  }

  TArray<AActor *> FoundActors;
  TArray<UStaticMeshComponent *> ActorComponents;
  UStaticMeshComponent *ReferenceLink = nullptr;
  GetOwner()->GetComponents(ActorComponents);

  for (UStaticMeshComponent *&Component : ActorComponents)
  {
    if (Component->GetName().Contains(CameraRef))
    {
      ReferenceLink = Component;
    }
  }
  if (!ReferenceLink)
  {
    UE_LOG(LogTemp, Error, TEXT("CameraRef not found"));
    return;
  }

  UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);

  for (auto &MyCamera : FoundActors)
  {
    if (MyCamera->GetName().Equals(CameraName))
    {
      MyCamera->AttachToComponent(ReferenceLink, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
      MyCamera->AddActorLocalOffset(PoseOffset.GetLocation());
      MyCamera->AddActorLocalRotation(PoseOffset.GetRotation());
      return;
    }
  }
  UE_LOG(LogTemp, Error, TEXT("Camera %s not found"), *CameraName);
}

void URCameraController::PerceiveObject()
{

  if (bActive)
  {
    GoalStatusList.Last().Status = 1;
    bool bObjectFound = false;
    // PerceivedActors.Empty();

    UE_LOG(LogTemp, Error, TEXT("a"));
    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(*TypeToPerceive), PerceivedActors);

    UE_LOG(LogTemp, Error, TEXT("b"));
    if (PerceivedActors.Num() > 0)
    {
      UE_LOG(LogTemp, Error, TEXT("6"));
      bObjectFound = true;
      PerceivedObject = NewObject<UPerceivedObject>(this);
      UE_LOG(LogTemp, Error, TEXT("7"));
      PerceivedObject->PoseWorld.SetLocation(PerceivedActors[0]->GetActorLocation());
      UE_LOG(LogTemp, Error, TEXT("8"));
      PerceivedObject->PoseWorld.SetRotation(PerceivedActors[0]->GetActorQuat());
      UE_LOG(LogTemp, Error, TEXT("9"));
      PerceivedObject->Name = PerceivedActors[0]->GetName();
      UE_LOG(LogTemp, Error, TEXT("10"));
    }

    // for(auto & Object : PerceivedObjects)
    //   {
    //     if(Object->Type.Equals(TypeToPerceive, ESearchCase::IgnoreCase))
    //       {
    //         PerceivedObject = Object;
    //         bObjectFound = true;
    //       }
    //   }

    if (bObjectFound)
    {

      UE_LOG(LogTemp, Error, TEXT("1"));
      URLink *ReferenceLink = GetOwner()->BaseLink;

      if(ReferenceLink)
        {
          FTransform ReferenceLinkTransform = ReferenceLink->GetCollision()->GetComponentTransform();
          FVector Location = ReferenceLinkTransform.GetLocation();
          Location.Z = 0.0f;
          ReferenceLinkTransform.SetLocation(Location);

          UE_LOG(LogTemp, Error, TEXT("2"));
          FVector Temp = PerceivedObject->PoseWorld.GetLocation() - ReferenceLinkTransform.GetLocation();
          FVector Pose = ReferenceLinkTransform.GetRotation().Inverse().RotateVector(Temp);

          UE_LOG(LogTemp, Error, TEXT("3"));
          PerceivedObject->Pose.SetLocation(Pose);
          FQuat TempRotator = PerceivedObject->PoseWorld.GetRotation() * ReferenceLinkTransform.GetRotation().Inverse();
          PerceivedObject->Pose.SetRotation(TempRotator);

          UE_LOG(LogTemp, Error, TEXT("4"));
          GoalStatusList.Last().Status = 3;
          bActive = false;
          bPublishResult = true;
          UE_LOG(LogTemp, Error, TEXT("5"));
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Robot BaseLink not set"));
        }
    }
    else
    {
      UE_LOG(LogTemp, Error, TEXT("No Object of Type %s found"), *TypeToPerceive);
    }
  }
}

void URCameraController::Tick(const float &InDeltaTime)
{
}
