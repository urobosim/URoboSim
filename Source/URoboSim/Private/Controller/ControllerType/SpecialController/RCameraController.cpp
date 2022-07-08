#include "Controller/ControllerType/SpecialController/RCameraController.h"

#include "Kismet/GameplayStatics.h"

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

  UPrimitiveComponent *ReferenceLink = nullptr;
  
  TArray<UPrimitiveComponent *> ActorComponents;
  GetOwner()->GetComponents(ActorComponents);

  for (UPrimitiveComponent *&Component : ActorComponents)
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

  TArray<AActor *> FoundActors;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), FoundActors);

  for (auto &MyCamera : FoundActors)
  {
    UE_LOG(LogTemp, Log, TEXT("Camera %s found. Check if target"), *MyCamera->GetName());
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
    bool bObjectFound = false;
    // PerceivedActors.Empty();

    UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName(*TypeToPerceive), PerceivedActors);

    if (PerceivedActors.Num() > 0)
    {
      bObjectFound = true;
      PerceivedObject = NewObject<UPerceivedObject>(this);
      PerceivedObject->PoseWorld.SetLocation(PerceivedActors[0]->GetActorLocation());
      PerceivedObject->PoseWorld.SetRotation(PerceivedActors[0]->GetActorQuat());
      PerceivedObject->Name = PerceivedActors[0]->GetName();
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

      URLink *ReferenceLink = GetOwner()->BaseLink;

      if(ReferenceLink)
        {
          FTransform ReferenceLinkTransform = ReferenceLink->GetCollision()->GetComponentTransform();
          FVector Location = ReferenceLinkTransform.GetLocation();
          Location.Z = 0.0f;
          ReferenceLinkTransform.SetLocation(Location);

          FVector Temp = PerceivedObject->PoseWorld.GetLocation() - ReferenceLinkTransform.GetLocation();
          FVector Pose = ReferenceLinkTransform.GetRotation().Inverse().RotateVector(Temp);

          PerceivedObject->Pose.SetLocation(Pose);
          FQuat TempRotator = PerceivedObject->PoseWorld.GetRotation() * ReferenceLinkTransform.GetRotation().Inverse();
          PerceivedObject->Pose.SetRotation(TempRotator);

          GoalStatusList.Last().Status = 3;
          bActive = false;
          bPublishResult = true;
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
