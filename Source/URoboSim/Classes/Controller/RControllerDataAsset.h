// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "Engine/DataAsset.h"
#include "Controller/RController.h"
#include "Containers/Map.h"
// clang-format off
#include "RControllerDataAsset.generated.h"
// clang-format on

USTRUCT()
struct FRControllerConfiguration
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, Instanced)
  TMap<FString, URControllerParameter*> ControllerParameters;
};

UCLASS(BlueprintType, Category = "Controller")
class UROBOSIM_API URControllerDataAsset : public UDataAsset
{
  GENERATED_BODY()

public:
  // Names of the robots for which this configration asset is used
  UPROPERTY(EditAnywhere, Category = "Basic Information")
  TArray<FString> RobotNames;

  UPROPERTY(EditAnywhere, Category = "Basic Information")
  FRControllerConfiguration ControllerConfiguration;
};