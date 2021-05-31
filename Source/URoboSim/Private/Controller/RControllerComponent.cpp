#include "Controller/RControllerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRControllerComponent, Log, All);

URControllerComponent::URControllerComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
  PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void URControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
  for (URController *&Controller : Controllers)
  {
    Controller->Tick(DeltaTime);
  }
}

void URControllerComponent::Init()
{
  if (ARModel *Owner = Cast<ARModel>(GetOwner()))
  {
    for (URController *&Controller : Controllers)
    {
      Controller->Init();
    }
  }
  else
  {
    UE_LOG(LogRControllerComponent, Error, TEXT("Owner of %s is not ARModel"), *GetName())
  }
}

URController *URControllerComponent::GetController(const FString &ControllerName) const
{
  URController *const *ControllerPtr = Controllers.FindByPredicate([&](URController *Controller){ return Controller->GetName().Contains(ControllerName); });
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