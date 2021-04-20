// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "RSensor.h"
#include "RPluginComponent.h"
// clang-format off
#include "RSensorComponent.generated.h"
// clang-format on

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URSensorComponent final : public URPluginComponent
{
  GENERATED_BODY()

public:
  URSensorComponent();

public:
  void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

public:
  void AddSensor(URSensor *Sensor) { Sensors.Add(Sensor); }

  URSensor *GetSensor(const FString &SensorName);

protected:
  void Init() override;

private:
  UPROPERTY(EditAnywhere, Instanced)
  TArray<URSensor *> Sensors;
};