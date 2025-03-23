#include "RGraspComponent.h"
#include "Physics/RModel.h"
#include "Physics/RLink.h"
#include "ROSCommunication/Publisher/RTFPublisher.h"
#include "URoboSimSettings.h"

URGraspComponent::URGraspComponent()
{
  InitSphereRadius(GraspRadius);
  SetGenerateOverlapEvents(true);
  SetEnableGravity(false);

  // SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel18);
  SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel3);
  SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
  SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel18, ECollisionResponse::ECR_Overlap);

  FString ConstraintName = TEXT("Constraint_") + GetName();
  if(GetWorld())
  {
    Constraint = NewObject<UPhysicsConstraintComponent>(this, FName(*ConstraintName));
    Constraint->SetupAttachment(this);
    Constraint->ConstraintInstance.SetLinearXLimit(ELinearConstraintMotion::LCM_Locked, 0);
    Constraint->ConstraintInstance.SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 0);
    Constraint->ConstraintInstance.SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 0);
    // Constraint->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0);
    // Constraint->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0);
    // Constraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0);
    Constraint->ConstraintInstance.SetAngularTwistLimit(EAngularConstraintMotion::ACM_Limited, 0);
    Constraint->ConstraintInstance.SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Limited, 0);
    Constraint->ConstraintInstance.SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Limited, 0);
    Constraint->ConstraintInstance.ProfileInstance.LinearLimit.bSoftConstraint = false;
    Constraint->ConstraintInstance.ProfileInstance.TwistLimit.bSoftConstraint = true;
    Constraint->ConstraintInstance.ProfileInstance.ConeLimit.bSoftConstraint = true;
    Constraint->ConstraintInstance.ProfileInstance.LinearLimit.Stiffness = 3000000;
    Constraint->ConstraintInstance.ProfileInstance.TwistLimit.Stiffness = 300;

    Constraint->ConstraintInstance.ProfileInstance.ConeLimit.Restitution = 0;
    Constraint->ConstraintInstance.ProfileInstance.TwistLimit.Restitution = 0;
    Constraint->ConstraintInstance.ProfileInstance.ConeLimit.Stiffness = 300;
    Constraint->ConstraintInstance.ProfileInstance.LinearLimit.Damping = 3000000;
    Constraint->ConstraintInstance.ProfileInstance.TwistLimit.Damping = 3000000;
    Constraint->ConstraintInstance.ProfileInstance.ConeLimit.Damping = 3000000;
    Constraint->ConstraintInstance.ProfileInstance.LinearLimit.ContactDistance = 3000000;
    Constraint->ConstraintInstance.ProfileInstance.TwistLimit.ContactDistance = 3000000;
    Constraint->ConstraintInstance.ProfileInstance.ConeLimit.ContactDistance = 3000000;

  }
}

void URGraspComponent::PrintBroadcastRelease(AActor* InActor)
{
    if(bDebugMode)
      {
        UE_LOG(LogTemp, Log, TEXT("%s[%s:%d]"), *GetName(), *FString(__FUNCTION__), __LINE__);
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
  OnComponentBeginOverlap.AddUniqueDynamic(this, &URGraspComponent::OnFixationGraspAreaBeginOverlap);
  OnComponentEndOverlap.AddUniqueDynamic(this, &URGraspComponent::OnFixationGraspAreaEndOverlap);
  const UURoboSimSettings* Settings = GetDefault<UURoboSimSettings>();
  if(Settings)
    {
      bDebugMode = Settings->bDebugMode;
    }
  OnObjectReleased.AddUniqueDynamic(this, &URGraspComponent::PrintBroadcastRelease);

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
  OnComponentBeginOverlap.AddUniqueDynamic(this, &URGraspComponent::OnFixationGraspAreaBeginOverlap);
  OnComponentEndOverlap.AddUniqueDynamic(this, &URGraspComponent::OnFixationGraspAreaEndOverlap);
  OnObjectReleased.AddUniqueDynamic(this, &URGraspComponent::PrintBroadcastRelease);
}

void URGraspComponent::BeginPlay()
{
  Super::BeginPlay();
  ComponentInReach = nullptr;
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
      if(!FixatedObject)
        {

          if(bDebugMode)
            {
              UE_LOG(LogTemp, Log, TEXT("%s: Object in Reach, overlap with %s / %s"), *GetName(), *OtherSMA->GetName(), *OtherComp->GetName());
            }
          ObjectsInReach.Emplace(OtherSMA);
          if(ComponentInReach == nullptr)
            {
              if(bDebugMode)
                {
                  UE_LOG(LogTemp, Log, TEXT("%s: Set ComponentInReach to %s"), *GetName(), *OtherComp->GetName());
                }
              ComponentInReach = OtherComp;
            }
        }
    }
}

void URGraspComponent::OnFixationGraspAreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
                                                     class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  // Remove actor from array (if present)
  if (AStaticMeshActor* SMA = Cast<AStaticMeshActor>(OtherActor))
    {
      if(bDebugMode)
        {
          UE_LOG(LogTemp, Log, TEXT("%s: Object %s / %s left Reach"), *GetName(), *SMA->GetName(), *OtherComp->GetName());
        }
      ObjectsInReach.Remove(SMA);
      if(ComponentInReach == OtherComp)
        {
          ComponentInReach = nullptr;
        }
    }
}

// Try to fixate object to hand
bool URGraspComponent::TryToFixate()
{
  bool bSuccess = false;

  // if(bObjectGrasped)
  if(ObjectsInReach.Num() > 0)
    {
      if(!bObjectGrasped)
        {
          // Pop a SMA
          AStaticMeshActor* SMA = ObjectsInReach[0];
          // Check if the actor is graspable
          FixateObject(SMA, ComponentInReach);

          if(bDebugMode)
            {
              UE_LOG(LogTemp, Log, TEXT("%s: Fixate Object %s / %s"), *GetName(), *SMA->GetName(), *ComponentInReach->GetName());
            }
        }
    }
  else
    {
      if(bDebugMode)
        {
          UE_LOG(LogTemp, Warning, TEXT("%s: No Object to grasp"), *GetName());
        }
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
  FixatedComponent = SMC;
  if(Gripper)
  {
    Constraint->ConstraintActor2 = ConstrainedActor;
    Constraint->SetConstrainedComponents(Gripper, NAME_None, SMC, NAME_None);
  }

  // if(OnObjectGrasped.IsBound())
  //   {
  //   }
  OnObjectGrasped.Broadcast(ConstrainedActor);
  bGraspObjectGravity = SMC->IsGravityEnabled();
  bObjectGrasped = true;
  SMC->SetEnableGravity(false);
}

// Detach fixation
void URGraspComponent::TryToDetach()
{
  if(bDebugMode)
    {
      UE_LOG(LogTemp, Log, TEXT("%s[%s:%d]"), *GetName(), *FString(__FUNCTION__), __LINE__);
    }
  if(FixatedObject)
  {
    if(bDebugMode)
      {
        UE_LOG(LogTemp, Log, TEXT("%s[%s:%d] asdf"), *GetName(), *FString(__FUNCTION__), __LINE__);
      }
    if(Gripper)
      {
        Constraint->BreakConstraint();
      }

    // if(OnObjectReleased.IsBound())
    // {

    // }
    OnObjectReleased.Broadcast(FixatedObject);
    if(bDebugMode)
      {
        UE_LOG(LogTemp, Log, TEXT("%s[%s:%d]"), *GetName(), *FString(__FUNCTION__), __LINE__);
      }
    //TODO: Fix bug where gravity is not enabled if left reach
    // ComponentInReach->SetEnableGravity(bGraspObjectGravity);
    //bug if grasped by multiple graspcomp, Gravity can already be disabled bEnableGravity is then false
    if(FixatedComponent)
      {
        FixatedComponent->SetEnableGravity(true);
      }
    else
      {
        UE_LOG(LogTemp, Error, TEXT("%s [%s:%d]: ComponentInReach was nullptr"), *GetName(), *FString(__FUNCTION__), __LINE__);
      }
    FixatedObject = nullptr;
    FixatedComponent = nullptr;
  }
  bObjectGrasped = false;
}
