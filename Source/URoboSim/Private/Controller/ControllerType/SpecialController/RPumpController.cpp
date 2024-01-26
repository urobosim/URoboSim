#include "Controller/ControllerType/SpecialController/RPumpController.h"
#include "RGraspComponent.h"

URPumpController::URPumpController()
{
}

void URPumpController::SetControllerParameters(URControllerParameter*& ControllerParameters)
{
	URPumpControllerParameter* PumpControllerParameters = Cast<URPumpControllerParameter>(
		ControllerParameters);
	if (PumpControllerParameters)
	{
          TrayReferenceFrame = PumpControllerParameters->TrayReferenceFrame;
          TrayReferenceLink = PumpControllerParameters->TrayReferenceLink;
          TraySlot1Frame = PumpControllerParameters->TraySlot1Frame;
          TraySlot2Frame = PumpControllerParameters->TraySlot2Frame;
          HolderReferenceFrame = PumpControllerParameters->HolderReferenceFrame;
          HolderFrame = PumpControllerParameters->HolderFrame;
          TrayPartNames.Append(PumpControllerParameters->TrayPartNames);
	}
}

UPrimitiveComponent* URPumpController::ParseChildFramesForRef(TArray<USceneComponent*> InChildFrames, FString RefName)
{
  UPrimitiveComponent* TempRef = nullptr;

  for(auto& ChildFrame : InChildFrames)
    {
      if(ChildFrame->GetName().Contains(RefName))
        {
          // UE_LOG(LogTemp, Log, TEXT("[%s]: Referenceframe %s"), *FString(__FUNCTION__), *ChildFrame->GetName());
          TempRef = Cast<UPrimitiveComponent>(ChildFrame);
        }
      else
        {
          TArray<USceneComponent*> ChildChildFrames = ChildFrame->GetAttachChildren();
          TempRef = ParseChildFramesForRef(ChildChildFrames, RefName);
        }
      if(TempRef)
        {
          break;
        }
    }

  return TempRef;
}

void URPumpController::Init()
{
        Super::Init();


        if (!GetOwner())
        {
                UE_LOG(LogTemp, Error, TEXT("%s not attached to ARModel"), *GetName());
	}
	else
	{
		URLink* Pump = GetOwner()->Links.FindRef(TrayReferenceLink);


		if (!Pump)
		{
			UE_LOG(LogTemp, Error, TEXT("Pump ReferenceLink %s of %s not found"), *TrayReferenceLink, *GetName());
			return;
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Pump ReferenceLink %s of %s found"), *TrayReferenceLink, *GetName());
		}

                TArray<USceneComponent*> ChildFrames = Pump->GetAttachChildren();
                Ref = ParseChildFramesForRef(ChildFrames, TrayReferenceFrame);

                if(!Ref)
                  {
                    UE_LOG(LogTemp, Error, TEXT("[%s]: TrayReferenceframe %s not found"), *FString(__FUNCTION__), *TrayReferenceFrame);
                  }
                else
                  {
                    // Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
                    // Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);

                    Ref->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
                    Tray1Overlap = SetupOverlap(FName(GetName() + TEXT("_Tray1Overlap")), TraySlot1Frame, Ref);
                    Tray2Overlap = SetupOverlap(FName(GetName() + TEXT("_Tray2Overlap")), TraySlot2Frame, Ref);
                  }

                HolderRef = ParseChildFramesForRef(ChildFrames, HolderReferenceFrame);
                if(!HolderRef)
                  {
                    UE_LOG(LogTemp, Error, TEXT("[%s]: HolderReferenceframe %s not found"), *FString(__FUNCTION__), *HolderReferenceFrame);
                  }
                else
                  {
                    // HolderRef->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
                    // HolderRef->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);

                    HolderRef->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
                    HolderOverlap = SetupOverlap(FName(GetName() + TEXT("_HolderOverlap")), HolderFrame, HolderRef, 5.0);
                  }

                // PumpHandler = NewObject<URPumpControllerHandler>(this);
                // if(!PumpHandler)
                //   {
                //     UE_LOG(LogTemp, Error, TEXT("[%s]: No Pump Handler"), *FString(__FUNCTION__));
                //   }

                TArray<UActorComponent*> TempComp;
                GetOwner()->GetComponents(URGraspComponent::StaticClass(), TempComp, true);

                for(auto& tmp : TempComp)
                  {
                    GraspComps.Add(Cast<URGraspComponent>(tmp));
                  }

                UPrimitiveComponent* TrayPartTemp = nullptr;
                for(auto& TrayPartName : TrayPartNames)
                  {
                    TrayPartTemp = Cast<UPrimitiveComponent>(ParseChildFramesForRef(ChildFrames, TrayPartName));
                    if(TrayPartTemp)
                      {
                        TrayParts.AddUnique(TrayPartTemp);
                        UE_LOG(LogTemp, Log, TEXT("[%s]: Traypart %s"), *FString(__FUNCTION__), *TrayPartName);
                      }
                    TrayPartTemp = nullptr;
                  }
 	}
}

USphereComponent* URPumpController::SetupOverlap(const FName& InName, const FVector& InOffset, UPrimitiveComponent* InReference, float InRadius)
{
  USphereComponent* Overlap = NewObject<USphereComponent>(GetOwner(), InName);
  Overlap->CreationMethod = EComponentCreationMethod::Instance;
  Overlap->RegisterComponent();
  Overlap->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel18);
  Overlap->SetGenerateOverlapEvents(true);
  Overlap->AttachToComponent(InReference, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
  Overlap->AddRelativeLocation(InOffset);
  Overlap->SetSphereRadius(InRadius, true);
  Overlap->SetHiddenInGame(false , false);
  Overlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel18, ECollisionResponse::ECR_Ignore);
  Overlap->OnComponentBeginOverlap.AddUniqueDynamic(this, &URPumpController::OnTrayAreaBeginOverlap);
  Overlap->OnComponentEndOverlap.AddUniqueDynamic(this, &URPumpController::OnTrayAreaEndOverlap);

  return Overlap;
}

void URPumpController::OnTrayAreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                             bool bFromSweep, const FHitResult& SweepResult)
{
  if(Cast<UStaticMeshComponent>(OtherComp))
    {
      UE_LOG(LogTemp, Log, TEXT("[%s]: Overlap with %s"), *FString(__FUNCTION__), *HitComp->GetName());

      for(auto& GraspComp : GraspComps)
        {
          GraspComp->OnObjectGrasped.AddUniqueDynamic(this, &URPumpController::ReleaseObject);


          if(!Tray1Overlap)
            {
              UE_LOG(LogTemp, Error, TEXT("[%s]:Tray1Overlap nullptr"), *FString(__FUNCTION__));
            }
          else if (HitComp->GetName().Equals(Tray1Overlap->GetName()))
            {
              GraspComp->OnObjectReleased.AddUniqueDynamic(this, &URPumpController::SetObjectTray1);
              DisableTrayCollision();
              break;
            }

          if(!Tray2Overlap)
            {
              UE_LOG(LogTemp, Error, TEXT("[%s]:Tray2Overlap nullptr"), *FString(__FUNCTION__));
            }
          else if(HitComp->GetName().Equals(Tray2Overlap->GetName()))
            {
              GraspComp->OnObjectReleased.AddUniqueDynamic(this, &URPumpController::SetObjectTray2);
              DisableTrayCollision();
              break;
            }

          if(!HolderOverlap)
            {
              UE_LOG(LogTemp, Error, TEXT("[%s]:HolderOverlap nullptr"), *FString(__FUNCTION__));
            }
          else if(HitComp->GetName().Equals(HolderOverlap->GetName()))
            {
              GraspComp->OnObjectReleased.AddUniqueDynamic(this, &URPumpController::SetObjectHolder);
              break;
            }

          UE_LOG(LogTemp, Error, TEXT("[%s]: Overlap neither Tray1, Tray2 or Holder but %s"), *FString(__FUNCTION__), *HitComp->GetName());
        }
    }
}

void URPumpController::OnTrayAreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  if(Cast<UStaticMeshComponent>(OtherComp))
    {
      UE_LOG(LogTemp, Log, TEXT("[%s]: End overlap with %s"), *FString(__FUNCTION__), *HitComp->GetName());

      UStaticMeshComponent* Root = Cast<UStaticMeshComponent>(OtherActor->GetRootComponent());
      if(Root)
        {
          Root->SetSimulatePhysics(true);
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Root of %s not a StaticMesh"), *OtherActor->GetName());
          return;
        }

      for(auto& GraspComp : GraspComps)
        {
          GraspComp->OnObjectGrasped.Remove(this, FName(TEXT("ReleaseObject")));


          if(!Tray1Overlap)
            {
              UE_LOG(LogTemp, Error, TEXT("[%s]:Tray1Overlap nullptr"), *FString(__FUNCTION__));
            }
          else if(HitComp->GetName().Equals(Tray1Overlap->GetName()))
            {
              EnableTrayCollision();
              GraspComp->OnObjectReleased.Remove(this, FName(TEXT("SetObjectTray1")));
              break;
            }

          if(!Tray2Overlap)
            {
              UE_LOG(LogTemp, Error, TEXT("[%s]:Tray2Overlap nullptr"), *FString(__FUNCTION__));
            }
          else if(HitComp->GetName().Equals(Tray2Overlap->GetName()))
            {

              EnableTrayCollision();
              GraspComp->OnObjectReleased.Remove(this, FName(TEXT("SetObjectTray2")));
              break;
            }

          if(!HolderOverlap)
            {
              UE_LOG(LogTemp, Error, TEXT("[%s]:HolderOverlap nullptr"), *FString(__FUNCTION__));
            }
          else if(HitComp->GetName().Equals(HolderOverlap->GetName()))
            {
              GraspComp->OnObjectReleased.Remove(this, FName(TEXT("SetObjectHolder")));
              break;
            }

          UE_LOG(LogTemp, Error, TEXT("[%s]: Overlap neither Tray1, Tray2 or Holder but %s"), *FString(__FUNCTION__), *HitComp->GetName());

        }
    }
}

void URPumpController::ReleaseObject(AActor* Object)
{
  UStaticMeshComponent* Root = Cast<UStaticMeshComponent>(Object->GetRootComponent());
  if(Root)
    {
      Root->SetSimulatePhysics(true);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Root of %s not a StaticMesh"), *Object->GetName());
      return;
    }
}

void URPumpController::SetObjectTray1(AActor* Object)
{
  UStaticMeshComponent* Root = Cast<UStaticMeshComponent>(Object->GetRootComponent());
  if(Root)
    {
      Root->SetSimulatePhysics(false);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Root of %s not a StaticMesh"), *Object->GetName());
      return;
    }
  Object->SetActorTransform(FTransform( FRotator(0, 0 , 0), Tray1Overlap->GetComponentLocation() + FVector(0, 0, 4.8), FVector(1.0, 1.0, 1.0)));
}

void URPumpController::SetObjectTray2(AActor* Object)
{
  UStaticMeshComponent* Root = Cast<UStaticMeshComponent>(Object->GetRootComponent());
  if(Root)
    {
      Root->SetSimulatePhysics(false);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Root of %s not a StaticMesh"), *Object->GetName());
    }

  Object->SetActorTransform(FTransform( FRotator(0, 0 , 0), Tray2Overlap->GetComponentLocation() + FVector(0, 0, 4.8), FVector(1.0, 1.0, 1.0)));
}

void URPumpController::SetObjectHolder(AActor* Object)
{
  UStaticMeshComponent* Root = Cast<UStaticMeshComponent>(Object->GetRootComponent());
  if(Root)
    {
      Root->SetSimulatePhysics(false);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Root of %s not a StaticMesh"), *Object->GetName());
    }

  Object->SetActorTransform(FTransform( FRotator(180, 0 , 0), HolderOverlap->GetComponentLocation() + FVector(0, 0, 2.0), FVector(1.0, 1.0, 1.0)));
}

void URPumpController::DisableTrayCollision()
{
  for(auto& TrayPart: TrayParts)
    {
      TrayPart->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    }
}

void URPumpController::EnableTrayCollision()
{
  for(auto& TrayPart: TrayParts)
    {
      TrayPart->SetCollisionProfileName("RobotWithSelfCollision");
    }
}
