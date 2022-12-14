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

                PumpHandler = NewObject<URPumpControllerHandler>(this);
                if(!PumpHandler)
                  {
                    UE_LOG(LogTemp, Error, TEXT("[%s]: No Pump Handler"), *FString(__FUNCTION__));
                    return;
                  }

		Tray1Overlap = NewObject<USphereComponent>(GetOwner(), FName(GetName() + TEXT("_Tray1Overlap")));
		Tray1Overlap->CreationMethod = EComponentCreationMethod::Instance;
		Tray1Overlap->RegisterComponent();
                Tray1Overlap->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel18);
                Tray1Overlap->SetGenerateOverlapEvents(true);
		Tray1Overlap->AttachToComponent(Ref, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Tray1Overlap->AddRelativeLocation(TraySlot1Frame);
                Tray1Overlap->SetSphereRadius(2.2,true);
                Tray1Overlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel18, ECollisionResponse::ECR_Ignore);
                Tray1Overlap->SetHiddenInGame(false, false);

                Tray1Overlap->OnComponentBeginOverlap.AddDynamic(this, &URPumpController::OnTray1AreaBeginOverlap);
                Tray1Overlap->OnComponentEndOverlap.AddDynamic(this, &URPumpController::OnTray1AreaEndOverlap);

		Tray1Constaint = NewObject<UPhysicsConstraintComponent>(GetOwner(), FName(GetName() + TEXT("_Tray1Constraint")));
		Tray1Constaint->CreationMethod = EComponentCreationMethod::Instance;
		Tray1Constaint->RegisterComponent();
		Tray1Constaint->AttachToComponent(Ref, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Tray1Constaint->AddRelativeLocation(TraySlot1Frame);
                Tray1Constaint->SetDisableCollision(true);

                Tray1Constaint->ConstraintInstance.SetLinearXLimit(ELinearConstraintMotion::LCM_Free, 0);
                Tray1Constaint->ConstraintInstance.SetLinearYLimit(ELinearConstraintMotion::LCM_Free, 0);
                Tray1Constaint->ConstraintInstance.SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 0);

		Tray2Overlap = NewObject<USphereComponent>(GetOwner(), FName(GetName() + TEXT("_Tray2Overlap")));
		Tray2Overlap->CreationMethod = EComponentCreationMethod::Instance;
		Tray2Overlap->RegisterComponent();
                Tray2Overlap->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel18);
                Tray2Overlap->SetGenerateOverlapEvents(true);
		Tray2Overlap->AttachToComponent(Ref, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Tray2Overlap->AddRelativeLocation(TraySlot2Frame);
                Tray2Overlap->SetSphereRadius(2.2, true);
                Tray2Overlap->SetHiddenInGame(false , false);
                Tray2Overlap->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel18, ECollisionResponse::ECR_Ignore);
                Tray2Overlap->OnComponentBeginOverlap.AddDynamic(this, &URPumpController::OnTray2AreaBeginOverlap);
                Tray2Overlap->OnComponentEndOverlap.AddDynamic(this, &URPumpController::OnTray2AreaEndOverlap);

		Tray2Constaint = NewObject<UPhysicsConstraintComponent>(GetOwner(), FName(GetName() + TEXT("_Tray2Constraint")));
		Tray2Constaint->CreationMethod = EComponentCreationMethod::Instance;
		Tray2Constaint->RegisterComponent();
		Tray2Constaint->AttachToComponent(Ref, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		Tray2Constaint->AddRelativeLocation(TraySlot2Frame);
                Tray2Constaint->SetDisableCollision(true);
                Tray2Constaint->ConstraintInstance.SetLinearXLimit(ELinearConstraintMotion::LCM_Free, 0);
                Tray2Constaint->ConstraintInstance.SetLinearYLimit(ELinearConstraintMotion::LCM_Free, 0);
                Tray2Constaint->ConstraintInstance.SetLinearZLimit(ELinearConstraintMotion::LCM_Free, 0);


                TArray<UActorComponent*> TempComp;
                GetOwner()->GetComponents(URGraspComponent::StaticClass(), TempComp, true);

                for(auto& tmp : TempComp)
                  {
                    GraspComps.Add(Cast<URGraspComponent>(tmp));
                  }
 	}
}

void URPumpController::OnTray1AreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                             bool bFromSweep, const FHitResult& SweepResult)
{
  if(Cast<UStaticMeshComponent>(OtherComp))
    {
      UE_LOG(LogTemp, Log, TEXT("[%s]"), *FString(__FUNCTION__));

      Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
      Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);

      // Tray1Constaint->SetConstrainedComponents(Ref, NAME_None, OtherComp, NAME_None);

      // Ref->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
      // OtherComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
      for(auto& GraspComp : GraspComps)
        {
          GraspComp->OnObjectGrasped.AddDynamic(this, &URPumpController::ReleaseConstraintTray1);
          GraspComp->OnObjectReleased.AddDynamic(this, &URPumpController::SetConstraintTray1);
        }
    }

}

void URPumpController::OnTray1AreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  if(Cast<UStaticMeshComponent>(OtherComp))
    {
      Tray1Constaint->BreakConstraint();
      UE_LOG(LogTemp, Log, TEXT("[%s]"), *FString(__FUNCTION__));

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
          GraspComp->OnObjectGrasped.Remove(this, FName(TEXT("ReleaseConstraintTray1")));
          GraspComp->OnObjectReleased.Remove(this, FName(TEXT("SetConstraintTray1")));
        }
    }
}

void URPumpController::OnTray2AreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                             bool bFromSweep, const FHitResult& SweepResult)
{
  if(Cast<UStaticMeshComponent>(OtherComp))
    {
      // OtherComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
      // Tray2Constaint->SetConstrainedComponents(Ref, NAME_None, OtherComp, NAME_None);
      UE_LOG(LogTemp, Log, TEXT("[%s]"), *FString(__FUNCTION__));
      for(auto& GraspComp : GraspComps)
        {
          GraspComp->OnObjectGrasped.AddDynamic(this, &URPumpController::ReleaseConstraintTray2);
          GraspComp->OnObjectReleased.AddDynamic(this, &URPumpController::SetConstraintTray2);
        }
    }
}

void URPumpController::OnTray2AreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  if(Cast<UStaticMeshComponent>(OtherComp))
    {
      UE_LOG(LogTemp, Log, TEXT("[%s]"), *FString(__FUNCTION__));
      Tray2Constaint->BreakConstraint();
      // Ref->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);
      for(auto& GraspComp : GraspComps)
        {
          GraspComp->OnObjectGrasped.Remove(this, FName(TEXT("ReleaseConstraintTray2")));
          GraspComp->OnObjectReleased.Remove(this, FName(TEXT("SetConstraintTray2")));
        }
    }

}

void URPumpController::SetConstraintTray1(AActor* Object)
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

  Object->SetActorTransform(FTransform( FRotator(0, 0 , 0), Tray1Overlap->GetComponentLocation(), FVector(1.0, 1.0, 1.0)));
}

void URPumpController::SetConstraintTray2(AActor* Object)
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
  Object->SetActorTransform(FTransform( FRotator(0, 0 , 0), Tray2Overlap->GetComponentLocation(), FVector(1.0, 1.0, 1.0)));
}

void URPumpController::ReleaseConstraintTray1(AActor* Object)
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

void URPumpController::ReleaseConstraintTray2(AActor* Object)
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
