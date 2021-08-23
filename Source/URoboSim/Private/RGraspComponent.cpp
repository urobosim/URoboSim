#include "RGraspComponent.h"
#include "Physics/RModel.h"
#include "Physics/RLink.h"
#include "ROSCommunication/Publisher/RTFPublisher.h"

URGraspComponent::URGraspComponent()
{
  InitSphereRadius(GraspRadius);
  SetGenerateOverlapEvents(true);
  SetEnableGravity(false);
  FString ConstraintName = TEXT("Constraint_") + GetName();
  Constraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName(*ConstraintName));
  Constraint->SetupAttachment(this);
  Constraint->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
  Constraint->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
  // Constraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 2);
  Constraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);

  FString ConstraintName2 = TEXT("Constraint2_") + GetName();
  Constraint2 = CreateDefaultSubobject<UPhysicsConstraintComponent>(FName(*ConstraintName2));
  Constraint2->SetupAttachment(this);
  Constraint2->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
  Constraint2->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
  // Constraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 2);
  Constraint2->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
}

void URGraspComponent::Init(UStaticMeshComponent* InGripper)
{
  Gripper = InGripper;
  bObjectGrasped = false;

  // TFPublisher = NewObject<URTFPublisher>(this, FName(*(GetName() + TEXT("_TFPublisher"))));
  // TFPublisher->Topic = TEXT("/tf_grasp_test");
  // TFPublisher->Init(TEXT("127.0.0.1"), 9090, this);

  OnComponentBeginOverlap.AddDynamic(this, &URGraspComponent::OnFixationGraspAreaBeginOverlap);
  OnComponentEndOverlap.AddDynamic(this, &URGraspComponent::OnFixationGraspAreaEndOverlap);

}

void URGraspComponent::Init(UStaticMeshComponent* InGripper1, UStaticMeshComponent* InGripper2)
{
  Gripper = InGripper1;
  Gripper2 = InGripper2;
  bObjectGrasped = false;

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
      // UE_LOG(LogTemp, Warning, TEXT("InReach %s"), *OtherSMA->GetName());
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

  // if(bObjectGrasped)
  if(!bObjectGrasped && ObjectsInReach.Num() > 0)
    {
      // Pop a SMA
      AStaticMeshActor* SMA = ObjectsInReach[0];

      // Check if the actor is graspable
      FixateObject(SMA);
    }

  //   {
  //     bSuccess = true;
  //   }

  // return bSuccess;
  return bObjectGrasped;
}

// Fixate object to hand
void URGraspComponent::FixateObject(AStaticMeshActor* InSMA)
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
          UE_LOG(LogTemp, Error, TEXT("ConstraintActor %s iter %d"),*ConstrainedActor->GetName(), NumIter);
          bParentFound = true;
        }
    }
  if(NumIter == 0)
  {
    ObjectToPublish = Cast<AActor>(ConstrainedActor);
  }

  UStaticMeshComponent* SMC = nullptr;
  SMC = ConstrainedActor->GetStaticMeshComponent();

  if(!SMC)
    {
      UE_LOG(LogTemp, Error, TEXT("RootComponent of InSMA has no static mesh"));
      return;
    }


  // if(InSMA == ConstrainedActor)
  //   {
  //     // Set the fixated object
  //     FixatedObject = ConstrainedActor;
  //     ConstrainedActor->GetStaticMeshComponent()->SetSimulatePhysics(false);
  //     ConstrainedActor->AttachToComponent(Gripper, FAttachmentTransformRules::KeepWorldTransform);
  //   }
  // else
  //   {
  //     Constraint->SetConstrainedComponents(Gripper, NAME_None, SMC, NAME_None);
  //   }

  FixatedObject = ConstrainedActor;
  if(Gripper)
  {
    Constraint->SetConstrainedComponents(Gripper, NAME_None, SMC, NAME_None);
  }

  if(Gripper2)
    {
      Constraint2->SetConstrainedComponents(Gripper2, NAME_None, SMC, NAME_None);
    }
  bGraspObjectGravity = SMC->IsGravityEnabled();
  bObjectGrasped = true;
  SMC->SetEnableGravity(false);


}

// Detach fixation
void URGraspComponent::TryToDetach()
{
  // if(FixatedObject)
  //   {
  //     FixatedObject->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
  //     FixatedObject->GetStaticMeshComponent()->SetSimulatePhysics(true);
  //     FixatedObject = nullptr;
  //   }
  // else
  //   {
  //     Constraint->BreakConstraint();
  //   }
  if(Gripper)
    {
      Constraint->BreakConstraint();
    }

  if(Gripper2)
    {
      Constraint2->BreakConstraint();
    }

  // if(ObjectToPublish)
  //   {
  //     UE_LOG(LogTemp, Error, TEXT("Start Publishing Object to Publish %s"), *ObjectToPublish->GetName());
  //     TFPublisher->AddObject(ObjectToPublish);
  //     TFPublisher->Publish();
  //   }

  if(FixatedObject)
  {

    FixatedObject->GetStaticMeshComponent()->SetEnableGravity(bGraspObjectGravity);
    FixatedObject = nullptr;
  }
  bObjectGrasped = false;
}
