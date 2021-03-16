// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "SDF/SDFGeometry.h"
// clang-format off
#include "SDFVisual.generated.h"
// clang-format on

UCLASS(BlueprintType, Category = "Visual")
class UROBOSIM_API USDFVisual : public UObject
{
	GENERATED_BODY()

public:
	// Constructor
	USDFVisual();

	UPROPERTY(EditAnywhere, Category = "Visual")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Visual")
	USDFGeometry *Geometry;
};
