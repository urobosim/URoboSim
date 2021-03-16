// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "SDF/SDFJoint.h"
#include "SDF/SDFLink.h"
// clang-format off
#include "SDFModel.generated.h"
// clang-format on

UCLASS(BlueprintType, Category = "Model")
class UROBOSIM_API USDFModel : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Model")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Model")
	bool bSelfColide;

	UPROPERTY(EditAnywhere, Category = "Model")
	bool bStatic;

	UPROPERTY(EditAnywhere, Category = "Model")
	bool bGravity;

	UPROPERTY(EditAnywhere, Category = "Model")
	TArray<USDFLink *> Links;

	UPROPERTY(EditAnywhere, Category = "Model")
	TArray<USDFJoint *> Joints;
};
