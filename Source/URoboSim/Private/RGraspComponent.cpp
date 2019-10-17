#include "RGraspComponent.h"
#include "Physics/RModel.h"
#include "Physics/RLink.h"


URGraspComponent::URGraspComponent()
{
  InitSphereRadius(10.f);
  SetGenerateOverlapEvents(true);
  bWeldFixation = true;
  ObjectMaxLength = 50.f;
  ObjectMaxMass = 15.f;
  // ConstComp = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("TESTConstraint"));
  // UE_LOG(LogTemp, Error, TEXT("Name %s"), *ConstComp->GetName());
}

void URGraspComponent::Init(URStaticMeshComponent* InGripper, TArray<URStaticMeshComponent*> InFixationPoints)
// void URGraspComponent::Init()
{
  Gripper = InGripper;
  // ConstComp->AttachToComponent(Gripper, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
  // ConstComp2->AttachToComponent(Gripper, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
  FixationPoints = InFixationPoints;



  UE_LOG(LogTemp, Error, TEXT("1  "));
  for(auto& FixPoint : FixationPoints)
    {
      UPhysicsConstraintComponent* TempConst = NewObject<UPhysicsConstraintComponent>(this);
      TempConst->AttachToComponent(FixPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
      TempConst->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
      TempConst->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
      ConstComps.Add(TempConst);
    }
  UE_LOG(LogTemp, Error, TEXT("2  "));
  OnComponentBeginOverlap.AddDynamic(this, &URGraspComponent::OnFixationGraspAreaBeginOverlap);
  OnComponentEndOverlap.AddDynamic(this, &URGraspComponent::OnFixationGraspAreaEndOverlap);

}

void URGraspComponent::BeginPlay()
{
  Super::BeginPlay();
}

void URGraspComponent::OnFixationGraspAreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                       class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
  UE_LOG(LogTemp, Error, TEXT("Gripper %s Overlap with Object: %s at %s"), *GetName(), *OtherActor->GetName(), *SweepResult.Location.ToString());
  if (ARModel* SMA = Cast<ARModel>(OtherActor))
    {
      return;
    }
  if (AStaticMeshActor* OtherSMA = Cast<AStaticMeshActor>(OtherActor))
    {
      ObjectsInReach.Emplace(OtherSMA);
    }
}



void URGraspComponent::OnFixationGraspAreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  // Remove actor from array (if present)
  if (AStaticMeshActor* SMA = Cast<AStaticMeshActor>(OtherActor))
    {
      ObjectsInReach.Remove(SMA);
    }
}

// Try to fixate object to hand
bool URGraspComponent::TryToFixate()
{
  bool bSuccess = false;
  if(!FixatedObject && ObjectsInReach.Num() > 0)
    {
      // Pop a SMA
      AStaticMeshActor* SMA = ObjectsInReach.Pop();

      // Check if the actor is graspable
      if (CanBeGrasped(SMA))
        {
          UE_LOG(LogTemp, Error, TEXT("Found Graspeble Object"));
          FixateObject(SMA);
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Object not a Graspeble Object"));
        }
    }

  if(FixatedObject)
    {
      UE_LOG(LogTemp, Error, TEXT("Grasp success"));
      bSuccess = true;
    }

  return bSuccess;
}

// Fixate object to hand
void URGraspComponent::FixateObject(AStaticMeshActor* InSMA)
{
  // Disable physics and overlap events
  UStaticMeshComponent* SMC = InSMA->GetStaticMeshComponent();


  for(int i = 0; i < ConstComps.Num(); i++)
  {
    ConstComps[i]->SetConstrainedComponents(FixationPoints[i], NAME_None, SMC, NAME_None);
  }

  // SMC->SetSimulatePhysics(false);

  // InSMA->AttachToComponent(Gripper, FAttachmentTransformRules(
  //                                                             EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, bWeldFixation));

  // Disable overlap checks during fixation grasp

  SetGenerateOverlapEvents(true);
  // Set the fixated object
  FixatedObject = InSMA;

  // Clear objects in reach array
  ObjectsInReach.Empty();
}

// Detach fixation
void URGraspComponent::TryToDetach()
{
  if (FixatedObject)
    {
      // Get current velocity before detachment (gets reseted)
      const FVector CurrVel = FixatedObject->GetVelocity();

      // Detach object from hand
      UStaticMeshComponent* SMC = FixatedObject->GetStaticMeshComponent();
      for(int i = 0; i < ConstComps.Num(); i++)
        {
          ConstComps[i]->SetConstrainedComponents(nullptr, NAME_None, nullptr, NAME_None);
        }
      // SMC->DetachFromComponent(FDetachmentTransformRules(
      //                                                    EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true));

      // Enable physics with and apply current hand velocity, clear pointer to object
      // SMC->SetSimulatePhysics(true);
      SMC->SetGenerateOverlapEvents(true);
      // SMC->SetPhysicsLinearVelocity(CurrVel);

      // Enable and update overlaps
      SetGenerateOverlapEvents(true);
      UpdateOverlaps();

      // Clear fixate object reference
      FixatedObject = nullptr;
    }

}

// Check if object is graspable
bool URGraspComponent::CanBeGrasped(AStaticMeshActor* InSMA)
{
  // Check if the object is movable
  if (!InSMA->IsRootComponentMovable())
    {
      return false;
    }

  // Check if actor has a static mesh component
  if (UStaticMeshComponent* SMC = InSMA->GetStaticMeshComponent())
    {
      // Check if component has physics on
      if (!SMC->IsSimulatingPhysics())
        {
          return false;

        }

      // Check if object fits size
      if (SMC->GetMass() < ObjectMaxMass
          && InSMA->GetComponentsBoundingBox().GetSize().Size() < ObjectMaxLength)
        {
          return true;

        }

    }
  return false;
}
