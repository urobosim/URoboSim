#include "RGraspComponent.h"
#include "Physics/RModel.h"
#include "Physics/RLink.h"

URGraspComponent::URGraspComponent()
{
  InitSphereRadius(10.f);
  SetGenerateOverlapEvents(true);
  SetEnableGravity(false);
  Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("Constraint"));
  Constraint->SetupAttachment(this);
  Constraint->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
  Constraint->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
  Constraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 2);
}

void URGraspComponent::Init(URStaticMeshComponent* InGripper)
{
  Gripper = InGripper;

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
      FixateObject(SMA);
    }

  if(FixatedObject)
    {
      bSuccess = true;
    }

  return bSuccess;
}

// Fixate object to hand
void URGraspComponent::FixateObject(AStaticMeshActor* InSMA)
{
  // Disable physics and overlap events

  AStaticMeshActor* ConstrainedActor = Cast<AStaticMeshActor>(InSMA->GetAttachParentActor());
  UStaticMeshComponent* SMC = nullptr;
  if(ConstrainedActor)
    {
      SMC = Cast<UStaticMeshComponent>(ConstrainedActor->GetRootComponent());
      if(!SMC)
        {
          UE_LOG(LogTemp, Error, TEXT("RootComponent of InSMA ParentActor not a StaticMesh Component"));
          return;
        }
      FixatedObject = ConstrainedActor;

    }
  else
    {
      SMC = InSMA->GetStaticMeshComponent();
      if(!SMC)
        {
          UE_LOG(LogTemp, Error, TEXT("RootComponent of InSMA not a StaticMesh Component"));
          return;
        }
      // Set the fixated object
      FixatedObject = InSMA;
    }

  Constraint->SetConstrainedComponents(Gripper, NAME_None, SMC, NAME_None);

  // Clear objects in reach array
  ObjectsInReach.Empty();

}

// Detach fixation
void URGraspComponent::TryToDetach()
{
  if (FixatedObject)
    {
      Constraint->BreakConstraint();
      FixatedObject = nullptr;
    }
}
