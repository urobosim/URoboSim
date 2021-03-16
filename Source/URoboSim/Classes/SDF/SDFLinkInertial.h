// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
// clang-format off
#include "SDFLinkInertial.generated.h"
// clang-format on

UCLASS(BlueprintType, Category = "Inertial")
class UROBOSIM_API USDFLinkInertial : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Inertial")
	float Mass;

	UPROPERTY(EditAnywhere, Category = "Inertial")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Inertial")
	FVector Inertia;
};
