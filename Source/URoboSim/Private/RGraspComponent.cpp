#include "RGraspComponent.h"
#include "Physics/RModel.h"
#include "Physics/RLink.h"
#include "ROSCommunication/Publisher/RTFPublisher.h"

URGraspComponent::URGraspComponent()
{
  InitSphereRadius(GraspRadius);
  SetGenerateOverlapEvents(true);
  SetEnableGravity(false);
  SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel18);
  SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
  SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel18, ECollisionResponse::ECR_Overlap);

  FString ConstraintName = TEXT("Constraint_") + GetName();
  if(GetWorld())
  {
    Constraint = NewObject<UPhysicsConstraintComponent>(this, FName(*ConstraintName));
    Constraint->SetupAttachment(this);
    Constraint->ConstraintInstance.ProfileInstance.LinearLimit.bSoftConstraint = true;
    Constraint->ConstraintInstance.ProfileInstance.LinearLimit.Restitution = 0;
    Constraint->ConstraintInstance.ProfileInstance.LinearLimit.Stiffness = 30000;
    Constraint->ConstraintInstance.ProfileInstance.TwistLimit.Stiffness = 30000;
    Constraint->ConstraintInstance.ProfileInstance.ConeLimit.Stiffness = 30000;
    Constraint->ConstraintInstance.ProfileInstance.LinearLimit.Damping = 30000;
    Constraint->ConstraintInstance.ProfileInstance.TwistLimit.Damping = 30000;
    Constraint->ConstraintInstance.ProfileInstance.ConeLimit.Damping = 30000;
    Constraint->ConstraintInstance.SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0);
    Constraint->ConstraintInstance.SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0);
    Constraint->ConstraintInstance.SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0);
    Constraint->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
    Constraint->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
    Constraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
  }
}

void URGraspComponent::OnComponentCreated()
{
  Super::OnComponentCreated();

}

void URGraspComponent::Init(UPrimitiveComponent* InGripper)
{
  Gripper = InGripper;
  bObjectGrasped = false;
  SetSphereRadius(GraspRadius, true);
  OnComponentBeginOverlap.AddDynamic(this, &URGraspComponent::OnFixationGraspAreaBeginOverlap);
  OnComponentEndOverlap.AddDynamic(this, &URGraspComponent::OnFixationGraspAreaEndOverlap);

}

void URGraspComponent::Init(UPrimitiveComponent* InGripper1, UPrimitiveComponent* InGripper2)
{
  Gripper = InGripper1;
  Gripper2 = InGripper2;
  bObjectGrasped = false;

  SetSphereRadius(GraspRadius, true);
  // TFPublisher = NewObject<URTFPublisher>(this, FName(*(GetName() + TEXT("_TFPublisher"))));
  // TFPublisher->Topic = TEXT("/tf_grasp_test");
  // TFPublisher->Init(TEXT("127.0.0.1"), 9090, this);
  OnComponentBeginOverlap.AddDynamic(this, &URGraspComponent::OnFixationGraspAreaBeginOverlap);
  OnComponentEndOverlap.AddDynamic(this, &URGraspComponent::OnFixationGraspAreaEndOverlap);

}

void URGraspComponent::BeginPlay()
{
  Super::BeginPlay();
}

void URGraspComponent::OverrideObjectInReach(AActor* InActor)
{
  if(AStaticMeshActor* Actor = Cast<AStaticMeshActor>(InActor))
    {
      ObjectsInReach.Emplace(Actor);
      ComponentInReach = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
    }
}

void URGraspComponent::OnFixationGraspAreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                       class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                       bool bFromSweep, const FHitResult & SweepResult)
{
  if (ARModel* SMA = Cast<ARModel>(OtherActor))
    {
      return;
    }
  if (AStaticMeshActor* OtherSMA = Cast<AStaticMeshActor>(OtherActor))
    {

      UE_LOG(LogTemp, Log, TEXT("%s: Object in Reach, overlap with %s"), *OtherSMA->GetName(), *OtherComp->GetName());
      ObjectsInReach.Emplace(OtherSMA);
      ComponentInReach = OtherComp;
    }
}

void URGraspComponent::OnFixationGraspAreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  // Remove actor from array (if present)
  if (AStaticMeshActor* SMA = Cast<AStaticMeshActor>(OtherActor))
    {
      UE_LOG(LogTemp, Log, TEXT("%s: Object left Reach"), *SMA->GetName());
      ObjectsInReach.Remove(SMA);
    }
}

// Try to fixate object to hand
bool URGraspComponent::TryToFixate()
{
  bool bSuccess = false;

  // if(bObjectGrasped)
  if(!bObjectGrasped && ObjectsInReach.Num() > 0)
    {
      // Pop a SMA
      AStaticMeshActor* SMA = ObjectsInReach[0];

      // Check if the actor is graspable
      FixateObject(SMA, ComponentInReach);
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("%s: No Object to grasp"), *GetName());
    }
  return bObjectGrasped;
}

// Fixate object to hand
void URGraspComponent::FixateObject(AStaticMeshActor* InSMA, UPrimitiveComponent* InSMC)
{
  // AStaticMeshActor* ConstrainedActor = Cast<AStaticMeshActor>(InSMA->GetAttachParentActor());
  AStaticMeshActor* ConstrainedActor = InSMA;

  //If the grasped object is attached to another object (door handle), connecting via constraints moves
  // the gripper to the root object
  bool bParentFound = false;
  int NumIter = 0;
  while(!bParentFound)
    {
      AStaticMeshActor* TempActor = Cast<AStaticMeshActor>(ConstrainedActor->GetAttachParentActor());
      if(TempActor)
        {
          ConstrainedActor = TempActor;
          NumIter++;
        }
      else
        {
          bParentFound = true;
        }
    }

  UPrimitiveComponent* SMC = nullptr;
  if(NumIter == 0)
  {
    ObjectToPublish = Cast<AActor>(ConstrainedActor);
    SMC = InSMC;
  }
  else
    {
      SMC = ConstrainedActor->GetStaticMeshComponent();
      ComponentInReach = SMC;
    }


  if(!SMC)
    {
      UE_LOG(LogTemp, Error, TEXT("Overlapping or Root component was no static mesh"));
      return;
    }


  FixatedObject = ConstrainedActor;
  if(Gripper)
  {
    Constraint->ConstraintActor2 = ConstrainedActor;
    Constraint->SetConstrainedComponents(Gripper, NAME_None, SMC, NAME_None);
  }

  if(OnObjectGrasped.IsBound())
    {
      OnObjectGrasped.Broadcast(ConstrainedActor);
    }
  bGraspObjectGravity = SMC->IsGravityEnabled();
  bObjectGrasped = true;
  SMC->SetEnableGravity(false);
}

// Detach fixation
void URGraspComponent::TryToDetach()
{
  if(FixatedObject)
  {

    if(Gripper)
      {
        Constraint->BreakConstraint();
      }

    if(OnObjectGrasped.IsBound())
    {
      OnObjectReleased.Broadcast(FixatedObject);
    }
    ComponentInReach->SetEnableGravity(bGraspObjectGravity);
    FixatedObject = nullptr;
  }
  bObjectGrasped = false;
}
