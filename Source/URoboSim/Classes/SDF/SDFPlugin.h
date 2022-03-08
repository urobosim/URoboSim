// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SDFPlugin.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "SDF")
class UROBOSIM_API USDFPlugin : public UObject
{
	GENERATED_BODY()

public:
	// Constructor
	USDFPlugin();

	UPROPERTY(EditAnywhere, Category = "Plugin")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Plugin")
	FString Joint;

	UPROPERTY(EditAnywhere, Category = "Plugin")
	FString MimicJoint;

	UPROPERTY(EditAnywhere, Category = "Plugin")
	float Multiplier;
};
