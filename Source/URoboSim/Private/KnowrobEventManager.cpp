#include "KnowrobEventManager.h"

void UEventManagerComponent::BeginPlay()
{
  Super::BeginPlay();
}

void UCollisionEventManager::RegisterEvent()
{
  // AActor* Object = GetOwner();
  if(Object)
    {
      Object->OnActorHit.AddDynamic(this, &UCollisionEventManager::Callback);
    }

}
// void UCollisionEventManager::Callback(FActorHitSignature InActorHitSignature)
void UCollisionEventManager::Callback(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{

}
