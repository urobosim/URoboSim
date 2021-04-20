#include "Factory/RSensorBuilder.h"
#include "Sensor/SensorType/RLidar2D.h"

DEFINE_LOG_CATEGORY_STATIC(LogRSensorBuilder, Log, All);

void URSensorBuilder::Init(const TArray<ARModel *> &InModels, const FRSensorConfiguration &InSensorConfiguration)
{
  if (InModels.Num() == 0)
  {
    UE_LOG(LogRSensorBuilder, Error, TEXT("No models found"))
    return;
  }

  if (InSensorConfiguration.SensorParameters.Num() == 0)
  {
    UE_LOG(LogRSensorBuilder, Error, TEXT("No SensorConfiguration found"))
    return;
  }

  Models = InModels;
  SensorConfiguration = InSensorConfiguration;
}

void URSensorBuilder::Build()
{
  for (ARModel *&Model : Models)
  {
    URSensorComponent *SensorComponent = NewObject<URSensorComponent>(Model, TEXT("SensorComponent"));
    
    for (TPair<FString, URSensorParameter *> SensorParameters : SensorConfiguration.SensorParameters)
    {
      UE_LOG(LogRSensorBuilder, Log, TEXT("Create %s of %s"), *SensorParameters.Key, *Model->GetName());
      URSensor *Sensor = CreateSensor(Model, SensorParameters);
      Sensor->SetSensorParameters(SensorParameters.Value);
      SensorComponent->AddSensor(Sensor);
    }
    SensorComponent->RegisterComponent();
  }
}

URSensor *URSensorBuilder::CreateSensor(ARModel *&InOwner, const TPair<FString, URSensorParameter *> SensorParameters)
{
  if (Cast<URLidar2DParameter>(SensorParameters.Value))
  {
    return NewObject<URLidar2D>(InOwner, *SensorParameters.Key);
  }
  else
  {
    return NewObject<URSensor>(InOwner, *SensorParameters.Key);
  }
}