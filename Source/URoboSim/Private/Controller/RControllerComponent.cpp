#include "Controller/RControllerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRControllerComponent, Log, All);

URControllerComponent::URControllerComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
  PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void URControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
  float realtimeSeconds = FPlatformTime::Seconds();
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
  for (URController *&Controller : Controllers)
  {
    Controller->Tick(DeltaTime);
  }
}
void URControllerComponent::BeginPlay()
{
  Super::BeginPlay();

  Init();
}

void URControllerComponent::Init()
{
  RegisterComponent(); // Remove this will cause the TickComponent not fire in Play
  if (!GetOwner())
  {
    UE_LOG(LogRControllerComponent, Error, TEXT("Owner of %s is not RModel"), *GetName())
  }
  else
  {
    for (URController *&Controller : Controllers)
    {
      Controller->SetOwner(GetOwner());
      Controller->Init();
    }
  }
}

URController *URControllerComponent::GetController(FString ControllerName) const
{
  URController *const *ControllerPtr = Controllers.FindByPredicate([&](URController *Controller){ return Controller->GetName().Equals(ControllerName); });
  if (ControllerPtr)
  {
    return *ControllerPtr;
  }
  else
  {
    UE_LOG(LogRControllerComponent, Error, TEXT("%s of %s not found"), *ControllerName, *GetName())
    return nullptr;
  }
}