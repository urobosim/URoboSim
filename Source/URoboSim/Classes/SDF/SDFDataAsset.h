// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SDF/SDFModel.h"
#include "SDFDataAsset.generated.h"

/**
 *
 */
UCLASS(BlueprintType, Category = "SDF")
class UROBOSIM_API USDFDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	//// Constructor, set default values
	//USDFDataAsset();

	// SDF Version
	UPROPERTY(EditAnywhere, Category = "SDF")
	FString Version;

	// Array of models
	UPROPERTY(EditAnywhere, Category = "SDF")
	TArray<USDFModel*> Models;

//#if WITH_EDITOR
//	//////// Begin UObject interface
//	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
//	//////// End of UObject interface
//#endif
};
