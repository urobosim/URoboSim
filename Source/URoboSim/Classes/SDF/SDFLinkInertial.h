// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SDFLinkInertial.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Category = "SDF")
class UROBOSIM_API USDFLinkInertial : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Link|Inertial")
	float Mass;
		
	UPROPERTY(EditAnywhere, Category = "Link|Inertial")
	FTransform Pose;	
};
