// Copyright 2021, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Engine/DataAsset.h"
#include "Sensor/RSensor.h"
#include "Containers/Map.h"
// clang-format off
#include "RSensorDataAsset.generated.h"
// clang-format on

USTRUCT()
struct FRSensorConfiguration
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, Instanced)
  TMap<FString, URSensorParameter*> SensorParameters;
};

UCLASS(BlueprintType, Category = "Sensor")
class UROBOSIM_API URSensorDataAsset : public UDataAsset
{
  GENERATED_BODY()

public:
  // Names of the robots for which this configration asset is used
  UPROPERTY(EditAnywhere, Category = "Basic Information")
  TArray<FString> RobotNames;

  UPROPERTY(EditAnywhere, Category = "Basic Information")
  FRSensorConfiguration SensorConfiguration;
};