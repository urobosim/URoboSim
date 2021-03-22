
// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Controller/RGripperController.h"
#include "RControllerDataAsset.generated.h"
/**
 *
 */

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew , Category = "Controller")
class UROBOSIM_API URControllerConfiguration : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, Category = "Basic Information")
          FString ControllerName = TEXT("None");

	UPROPERTY(EditAnywhere, Category = "Basic Information")
          TSubclassOf<URController> ControllerType;
};

// Configuratation for spezific Controller
UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew , Category = "Controller")
class UROBOSIM_API URGripperConfiguration : public URControllerConfiguration
{
	GENERATED_BODY()
public:
  URGripperConfiguration(){ControllerType = URGripperController::StaticClass();};

};

UCLASS(BlueprintType, Category = "Controller")
class UROBOSIM_API URControllerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	//// Constructor, set default values

	// Names of the robots for which this configration asset is used
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Basic Information")
	TArray<FString> RobotNames;

	UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear, Category = "Basic Information")
	TArray<URControllerConfiguration*> ControllerConfigurations;

};
