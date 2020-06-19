// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SDF/SDFGeometry.h"
#include "SDFCollision.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "SDF")
class UROBOSIM_API USDFCollision : public UObject
{
	GENERATED_BODY()

public:
	// Constructor
	USDFCollision();

	UPROPERTY(EditAnywhere, Category = "Collision")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Collision")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Collision|Geometry")
	USDFGeometry* Geometry;
};
