// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SDF/SDFLink.h"
#include "SDF/SDFJoint.h"
#include "SDF/SDFPlugin.h"
#include "SDFModel.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "SDF")
class UROBOSIM_API USDFModel : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Model")
	FString Name;

	UPROPERTY(EditAnywhere, Category = "Model")
	FTransform Pose;

	UPROPERTY(EditAnywhere, Category = "Model")
	bool bStatic;

	UPROPERTY(EditAnywhere, Category = "Model")
	TArray<USDFLink*> Links;

	UPROPERTY(EditAnywhere, Category = "Model")
	TArray<USDFJoint*> Joints;

	UPROPERTY(EditAnywhere, Category = "Model")
	TArray<USDFPlugin*> Plugins;
};
