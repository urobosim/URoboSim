// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SDF/SDFGeometry.h"
#include "SDFVisual.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "SDF")
class UROBOSIM_API USDFVisual : public UObject
{
	GENERATED_BODY()

public:
	// Constructor
	USDFVisual();

	UPROPERTY(EditAnywhere, Category = "Visual")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Visual")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Visual|Geometry")
	USDFGeometry* Geometry;
};
