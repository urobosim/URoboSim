// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "SDF/SDFCollision.h"
#include "SDF/SDFLinkInertial.h"
#include "SDF/SDFVisual.h"
// clang-format on
#include "SDFLink.generated.h"
// clang-format off

UCLASS(BlueprintType, Category = "Link")
class UROBOSIM_API USDFLink : public UObject
{
	GENERATED_BODY()

public:
	// Constructor
	USDFLink();

	UPROPERTY(EditAnywhere, Category = "Link")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Link")
	bool bSelfCollide;

	UPROPERTY(EditAnywhere, Category = "Link")
	bool bGravity;

	UPROPERTY(EditAnywhere, Category = "Link")
	USDFLinkInertial* Inertial;

	UPROPERTY(EditAnywhere, Category = "Link")
	TArray<USDFCollision*> Collisions;

	UPROPERTY(EditAnywhere, Category = "Link")
	TArray<USDFVisual*> Visuals;
};
