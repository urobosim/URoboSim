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
	}
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
                for(auto& ChildFrame : ChildFrames)
                  {
                    if(ChildFrame->GetName().Contains(TrayReferenceFrame))
                      {
                        Ref = Cast<UPrimitiveComponent>(ChildFrame);
                      }
                  }

                if(!Ref)
                  {
                    UE_LOG(LogTemp, Error, TEXT("[%s]: Referenceframe %s not found"), *FString(__FUNCTION__), *TrayReferenceFrame);
                    return;
                  }
                else
                  {

                    UE_LOG(LogTemp, Log, TEXT("[%s]: Referenceframe %s"), *FString(__FUNCTION__), *TrayReferenceFrame);
                  }

                PumpHandler = NewObject<URPumpControllerHandler>(this);
                if(!PumpHandler)
                  {
                    UE_LOG(LogTemp, Error, TEXT("[%s]: No Pump Handler"), *FString(__FUNCTION__));
                    return;
                  }

                Tray1Overlap = SetupOverlap(FName(GetName() + TEXT("_Tray1Overlap")), TraySlot1Frame);
                Tray2Overlap = SetupOverlap(FName(GetName() + TEXT("_Tray2Overlap")), TraySlot2Frame);

                TArray<UActorComponent*> TempComp;
                GetOwner()->GetComponents(URGraspComponent::StaticClass(), TempComp, true);

                for(auto& tmp : TempComp)
                  {
                    GraspComps.Add(Cast<URGraspComponent>(tmp));
                  }
 	}
}

USphereComponent* URPumpController::SetupOverlap(const FName& InName, const FVector& InOffset)
{
  USphereComponent* TrayOverlap = NewObject<USphereComponent>(GetOwner(), InName);
  TrayOverlap->CreationMethod = EComponentCreationMethod::Instance;
  TrayOverlap->RegisterComponent();
  TrayOverlap->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel18);
  TrayOverlap->SetGenerateOverlapEvents(true);
  TrayOverlap->AttachToComponent(Ref, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
  TrayOverlap->AddRelativeLocation(InOffset);
  TrayOverlap->SetSphereRadius(2.2, true);
  TrayOverlap->SetHiddenInGame(false , false);
  TrayOverlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel18, ECollisionResponse::ECR_Ignore);
  TrayOverlap->OnComponentBeginOverlap.AddDynamic(this, &URPumpController::OnTrayAreaBeginOverlap);
  TrayOverlap->OnComponentEndOverlap.AddDynamic(this, &URPumpController::OnTrayAreaEndOverlap);

  return TrayOverlap;
}

void URPumpController::OnTrayAreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                             bool bFromSweep, const FHitResult& SweepResult)
{
  if(Cast<UStaticMeshComponent>(OtherComp))
    {
      UE_LOG(LogTemp, Log, TEXT("[%s]: Overlap with %s"), *FString(__FUNCTION__), *HitComp->GetName());

      if(Ref)
        {
          Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
          Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
        }

      if(!Tray1Overlap)
        {
          UE_LOG(LogTemp, Error, TEXT("[%s]:Tray1Overlap nullptr"), *FString(__FUNCTION__));
        }

      if(!Tray2Overlap)
        {
          UE_LOG(LogTemp, Error, TEXT("[%s]:Tray2Overlap nullptr"), *FString(__FUNCTION__));
        }
      for(auto& GraspComp : GraspComps)
        {
          GraspComp->OnObjectGrasped.AddDynamic(this, &URPumpController::ReleaseObject);


          if(HitComp->GetName().Equals(Tray1Overlap->GetName()))
            {
              GraspComp->OnObjectReleased.AddDynamic(this, &URPumpController::SetObjectTray1);
            }
          else if(HitComp->GetName().Equals(Tray2Overlap->GetName()))
            {
              GraspComp->OnObjectReleased.AddDynamic(this, &URPumpController::SetObjectTray2);
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("[%s]: Overlap neither Tray1 or Tray2 but %s"), *FString(__FUNCTION__), *HitComp->GetName());
            }
        }
    }
}

void URPumpController::OnTrayAreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  if(Cast<UStaticMeshComponent>(OtherComp))
    {
      UE_LOG(LogTemp, Log, TEXT("[%s]: End overlap with %s"), *FString(__FUNCTION__), *HitComp->GetName());

      Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
      Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);

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

          if(HitComp->GetName().Equals(Tray1Overlap->GetName()))
            {
              GraspComp->OnObjectReleased.Remove(this, FName(TEXT("SetObjectTray1")));
            }
          else if(HitComp->GetName().Equals(Tray2Overlap->GetName()))
            {
              GraspComp->OnObjectReleased.Remove(this, FName(TEXT("SetObjectTray2")));
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("[%s]: Overlap neither Tray1 or Tray2 but %s"), *FString(__FUNCTION__), *HitComp->GetName());
            }
        }
    }
}

void URPumpController::ReleaseObject(AActor* Object)
{

  Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
  Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Block);

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

  Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
  Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);

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

  Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
  Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
  Object->SetActorTransform(FTransform( FRotator(0, 0 , 0), Tray2Overlap->GetComponentLocation() + FVector(0, 0, 4.8), FVector(1.0, 1.0, 1.0)));
}
