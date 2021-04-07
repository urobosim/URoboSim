#include "Controller/RControllerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRControllerComponent, Log, All);

URControllerComponent::URControllerComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
  PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void URControllerComponent::Init()
{
  for (URController *&Controller : Controllers)
  {   
    Controller->SetOwner(GetOwner());
    Controller->Init();
  }
}

void URControllerComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
  for (URController *&Controller : Controllers)
  {
    Controller->Tick(DeltaTime);
  }
}

URController *URControllerComponent::GetController(const FString &ControllerName)
{
  URController *const *ControllerPtr = Controllers.FindByPredicate([&](URController *Controller){ return Controller->GetName().Equals(ControllerName); });
  if (ControllerPtr)
  {
    return *ControllerPtr;
  }
  else
  {
    UE_LOG(LogRControllerComponent, Error, TEXT("Controller %s not found in %s"), *ControllerName, *GetName())
    return nullptr;
  }
}