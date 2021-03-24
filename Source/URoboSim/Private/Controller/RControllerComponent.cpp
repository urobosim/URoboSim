#include "Controller/RControllerComponent.h"

URControllerComponent::URControllerComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
  PrimaryComponentTick.TickGroup = TG_PrePhysics;
  if (!GetOwner())
  {
    UE_LOG(LogTemp, Error, TEXT("Owner of %s is not RModel."), *GetName());
  }
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
  return *Controllers.FindByPredicate([&](URController *Controller){ return Controller->GetName().Equals(ControllerName); });
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