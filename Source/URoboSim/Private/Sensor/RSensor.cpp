#include "Sensor/RSensor.h"
#include "Sensor/RSensorComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRSensor, Log, All);

void URSensor::Init()
{
  UE_LOG(LogRSensor, Log, TEXT("Initialize %s"), *GetName())
  if (!Owner)
  {
    if (Cast<ARModel>(GetOuter()))
    {
      Owner = Cast<ARModel>(GetOuter());
    }
    else if (Cast<URSensorComponent>(GetOuter()) && Cast<ARModel>(Cast<URSensorComponent>(GetOuter())->GetOwner()))
    {
      Owner = Cast<ARModel>(Cast<URSensorComponent>(GetOuter())->GetOwner());
    }
  }
  if (!Owner)
  {
    UE_LOG(LogRSensor, Error, TEXT("Owner of %s not found, Outer is %s"), *GetName(), *GetOuter()->GetName())
  }
}

ARModel *URSensor::GetOwner()
{
  if (!Owner)
  {
    UE_LOG(LogRSensor, Error, TEXT("%s is not initialized"), *GetName())
  }
  return Owner;
}