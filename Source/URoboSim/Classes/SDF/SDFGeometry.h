// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "UObject/NoExportTypes.h"
#include "SDFGeometry.generated.h"

/**
*
*/
UENUM(BlueprintType, Category = "SDF")
enum class ESDFGeometryType : uint8
{
	None			UMETA(DisplayName = "None"),
	Mesh			UMETA(DisplayName = "Mesh"),
	Box				UMETA(DisplayName = "Box"),
	Cylinder		UMETA(DisplayName = "Cylinder"),
	Sphere			UMETA(DisplayName = "Sphere"),
};

/**
 *
 */
UCLASS(BlueprintType, Category = "SDF")
class UROBOSIM_API USDFGeometry : public UObject
{
	GENERATED_BODY()

public:
	USDFGeometry();

	UPROPERTY(EditAnywhere, Category = "Geometry")
	ESDFGeometryType Type;

	UPROPERTY(EditAnywhere, Category = "Geometry|Mesh")
	FString Uri;

	UPROPERTY(EditAnywhere, Category = "Geometry|Mesh")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, Category = "Geometry|Box")
	FVector Size;

	UPROPERTY(EditAnywhere, Category = "Geometry|Cylinder or Sphere")
	float Radius;

	UPROPERTY(EditAnywhere, Category = "Geometry|Cylinder")
	float Length;
};
