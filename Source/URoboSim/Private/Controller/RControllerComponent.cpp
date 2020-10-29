#include "Controller/RControllerComponent.h"

URControllerComponent::URControllerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

URControllerComponent::~URControllerComponent()
{
}

FString URControllerComponent::GetPluginName()
{
  return TEXT("ControllerComponent");
}

void URControllerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  float realtimeSeconds = FPlatformTime::Seconds();
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
  for(auto& C : Controller.ControllerList)
    {
      C.Value->Tick(DeltaTime);
    }
}
void URControllerComponent::BeginPlay()
{
  Super::BeginPlay();

  if(!GetOwner())
    {
      UE_LOG(LogTemp, Error, TEXT("Owner is no RModel."));
    }
  else
    {
      for(auto& C : Controller.ControllerList)
        {
          C.Value->SetOwner(GetOwner());
          C.Value->Init();
        }
    }
}

void URControllerComponent::SetJointVelocities(TArray<FString> InJointNames, TArray<float> InJointVelocities)
{
  for(int i = 0; i < InJointNames.Num();i++)
    {
      GetOwner()->Joints[InJointNames[i]]->SetJointVelocity(InJointVelocities[i]);
    }
}

URController* URControllerComponent::ControllerList(FString ControllerName)
{
  if(Controller.ControllerList.Contains(ControllerName))
    {
      return Controller.ControllerList[ControllerName];
    }
  return nullptr;
}
