#include "KnowrobEventManager.h"

void UEventManagerComponent::BeginPlay()
{
  Super::BeginPlay();
  URoboSimGameInstance* GI = Cast<URoboSimGameInstance>(UGameplayStatics::GetGameInstance(this));
  if(GI)
    {
      for(auto& EventManager : EventManagers)
        {
          EventManager->Init(GI->KnowrobInterface, GetOwner());
        }
    }
}


void UEventManager::Init(UKnowrobInterface* InKnowrobInterface, UObject* InObject)
{
  KnowrobInterface = InKnowrobInterface;
  SetObject(InObject);
  RegisterEvent();
}


void UCollisionEventManager::SetObject(UObject* InObject)
{
  Object = Cast<AActor>(InObject);
}

void UCollisionEventManager::RegisterEvent()
{
  if(Object)
    {
      Object->OnActorHit.AddDynamic(this, &UCollisionEventManager::Callback);
    }
}

void UCollisionEventManager::Callback(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
  if(KnowrobInterface)
    {
      if(UGameplayStatics::GetTimeSeconds(SelfActor) - LastCallback >= RepitionDelay)
        {
          LastCallback = UGameplayStatics::GetTimeSeconds(SelfActor);
          UE_LOG(LogTemp, Error, TEXT("Knowrobo callback"));

          FString Query = TEXT("get_time(Now),tell([is_event(E),has_type(E,failure:'PlacementFailure'),is_diagnosis(D), has_type(D, failure:'CollisionWithMovableObject'),has_participant(E, soma:'")
            + SelfActor->GetName() + TEXT("') ,has_participant(E, soma:'") + OtherActor->GetName() +
            TEXT("'), is_time_interval(Interval),holds(Interval, soma:hasIntervalBegin, Now),holds(Interval, soma:hasIntervalEnd, Now), occurs(E) during [Now, Now]]).");

          FString Id = TEXT("Collision_")  +  SelfActor->GetName() + TEXT("_") + OtherActor->GetName() + TEXT("_") + FString::FromInt(FMath::RandRange(0, 9999999));
          KnowrobInterface->QueryClient->SendQuery(0, Id , Query, SelfActor->GetWorldTimerManager());
        }
    }
}

 // I am not sure how is the milk bottle defined. Use the object instance of the milk bottle created before instead of O1
 // For the garbage can we can define a new object in our owl file. Do you have an owl file from Sebastian?

 // Assign value to 'Begin' when the event starts
