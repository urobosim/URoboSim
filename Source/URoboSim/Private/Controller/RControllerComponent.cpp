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

// void URControllerComponent::SetJointVelocities(TArray<FString> InJointNames, TArray<float> InJointVelocities)
// {
//   for (int i = 0; i < InJointNames.Num(); i++)
//   {
//     GetOwner()->Joints[InJointNames[i]]->SetJointVelocity(InJointVelocities[i]);
//   }
// }

// URController *URControllerComponent::ControllerList(FString ControllerName)
// {
//   if (Controller.ControllerList.Contains(ControllerName))
//   {
//     return Controller.ControllerList[ControllerName];
//   }
//   return nullptr;
// }