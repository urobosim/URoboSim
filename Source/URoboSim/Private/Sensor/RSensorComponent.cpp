#include "Sensor/RSensorComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRSensorComponent, Log, All);

URSensorComponent::URSensorComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
  PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void URSensorComponent::Init()
{
  for (URSensor *&Sensor : Sensors)
  {   
    Sensor->Init();
  }
}

void URSensorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
  for (URSensor *&Sensor : Sensors)
  {   
    Sensor->Tick(DeltaTime);
  }
}

URSensor *URSensorComponent::GetSensor(const FString &SensorName)
{
  URSensor *const *SensorPtr = Sensors.FindByPredicate([&](URSensor *Sensor){ return Sensor->GetName().Equals(SensorName); });
  if (SensorPtr)
  {
    return *SensorPtr;
  }
  else
  {
    UE_LOG(LogRSensorComponent, Error, TEXT("Sensor %s not found in %s"), *SensorName, *GetName())
    return nullptr;
  }
}