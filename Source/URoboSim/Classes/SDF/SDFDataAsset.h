// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "Engine/DataAsset.h"
#include "SDF/SDFModel.h"
// clang-format off
#include "SDFDataAsset.generated.h"
// clang-format on

UCLASS(BlueprintType, Category = "SDF")
class UROBOSIM_API USDFDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "SDF")
	FString Version;

	UPROPERTY(EditAnywhere, Category = "SDF")
	USDFModel *Model;
};
