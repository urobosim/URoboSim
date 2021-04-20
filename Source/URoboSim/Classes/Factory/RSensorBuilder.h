// Copyright 2021, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Sensor/RSensorComponent.h"
#include "Sensor/RSensorDataAsset.h"
// clang-format off
#include "RSensorBuilder.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URSensorBuilder : public UObject
{
  GENERATED_BODY()

public:
  virtual void Init(const TArray<ARModel *> &InModels, const FRSensorConfiguration &InSensorConfiguration);

  virtual void Build();

protected:
  virtual URSensor *CreateSensor(ARModel *&InOwner, const TPair<FString, URSensorParameter *> SensorParameters);

protected:
  TArray<ARModel *> Models;

  FRSensorConfiguration SensorConfiguration;
};