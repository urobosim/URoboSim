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
  for (auto &Controller : Controllers.ControllerList)
  {
    Controller.Value->Tick(DeltaTime);
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
    for (auto &Controller : Controllers.ControllerList)
    {
      Controller.Value->SetOwner(GetOwner());
      Controller.Value->Init();
    }
  }
}

void URControllerComponent::SetJointVelocities(TArray<FString> InJointNames, TArray<float> InJointVelocities)
{
  for (int i = 0; i < InJointNames.Num(); i++)
  {
    GetOwner()->Joints[InJointNames[i]]->SetJointVelocity(InJointVelocities[i]);
  }
}

URController *URControllerComponent::GetController(FString ControllerName) const
{
  if (Controllers.ControllerList.Contains(ControllerName))
  {
    return Controllers.ControllerList[ControllerName];
  }
  else
  {
    UE_LOG(LogRControllerComponent, Error, TEXT("%s of %s not found"), *ControllerName, *GetName())
    return nullptr;
  }
}
