// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SDFJointAxis.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "SDF", DefaultToInstanced, hidecategories = Object, editinlinenew)
class UROBOSIM_API USDFJointAxis : public UObject
{
	GENERATED_BODY()

public:

	USDFJointAxis();

	UPROPERTY(EditAnywhere, Category = "Joint|Axis")
	FVector Xyz;

	UPROPERTY(EditAnywhere, Category = "Joint|Axis")
	bool bUseParentModelFrame;

	UPROPERTY(EditAnywhere, Category = "Joint|Axis|Limit")
	float Lower;

	UPROPERTY(EditAnywhere, Category = "Joint|Axis|Limit")
	float Upper;

	UPROPERTY(EditAnywhere, Category = "Joint|Axis|Limit")
	float Effort;

	UPROPERTY(EditAnywhere, Category = "Joint|Axis|Limit")
	float Velocity;

	void SetLowerLimitFromSDF(float LowerLimit);
	void SetUpperLimitFromSDF(float UpperLimit);
};
