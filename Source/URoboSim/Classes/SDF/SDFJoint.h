// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "SDF/SDFJointAxis.h"
// clang-format on
#include "SDFJoint.generated.h"
// clang-format off

UCLASS(BlueprintType, Category = "Joint")
class UROBOSIM_API USDFJoint : public UObject
{
	GENERATED_BODY()

public:
	//Constructor
	USDFJoint();

	UPROPERTY(EditAnywhere, Category = "Joint")
	FString Type;

	UPROPERTY(EditAnywhere, Category = "Joint")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Joint")
	FString Parent;

	UPROPERTY(EditAnywhere, Category = "Joint")
	FString Child;

	UPROPERTY(EditAnywhere, Category = "Joint")
	USDFJointAxis *Axis;
};
