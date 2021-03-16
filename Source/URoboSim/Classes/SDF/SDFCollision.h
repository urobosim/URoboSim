// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once
#include "SDF/SDFGeometry.h"
// clang-format off
#include "SDFCollision.generated.h"
// clang-format on

UCLASS(BlueprintType, Category = "Collision")

class UROBOSIM_API USDFCollision : public UObject
{

	GENERATED_BODY()

public:
	// Constructor
	USDFCollision();

	UPROPERTY(EditAnywhere, Category = "Collision")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Collision")
	USDFGeometry *Geometry;
};