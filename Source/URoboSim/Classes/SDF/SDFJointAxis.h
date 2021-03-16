// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
// clang-format on
#include "SDFJointAxis.generated.h"
// clang-format off

UCLASS(BlueprintType, Category = "Axis")
class UROBOSIM_API USDFJointAxis : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Axis")
	float InitialPosition;

	UPROPERTY(EditAnywhere, Category = "Axis")
	FVector Xyz;

	UPROPERTY(EditAnywhere, Category = "Axis")
	bool bUseParentModelFrame;

	UPROPERTY(EditAnywhere, Category = "Axis|Limit")
	float Lower;

	UPROPERTY(EditAnywhere, Category = "Axis|Limit")
	float Upper;

	UPROPERTY(EditAnywhere, Category = "Axis|Limit")
	float Effort;

	UPROPERTY(EditAnywhere, Category = "Axis|Limit")
	float Velocity;
};
