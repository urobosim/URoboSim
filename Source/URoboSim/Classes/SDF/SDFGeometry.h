// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
// clang-format off
#include "SDFGeometry.generated.h"
// clang-format on

UENUM()
enum class ESDFGeometryType : uint8
{
	None UMETA(DisplayName = "None"),
	Mesh UMETA(DisplayName = "Mesh"),
	Box UMETA(DisplayName = "Box"),
	Cylinder UMETA(DisplayName = "Cylinder"),
	Sphere UMETA(DisplayName = "Sphere"),
};

UCLASS(BlueprintType, Category = "Geometry")
class UROBOSIM_API USDFGeometry : public UObject
{
	GENERATED_BODY()

public:
	ESDFGeometryType Type;

	UPROPERTY(VisibleAnywhere, Category = "GeometryMesh")
	UStaticMesh *Mesh;
};

UCLASS(BlueprintType, Category = "Geometry")
class UROBOSIM_API USDFGeometryMesh : public USDFGeometry
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "GeometryBox")
	FString Uri;

	UPROPERTY(VisibleAnywhere, Category = "GeometryBox")
	FVector Scale;
};

UCLASS(BlueprintType, Category = "Geometry")
class UROBOSIM_API USDFGeometryBox : public USDFGeometry
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	FVector Size;
};

UCLASS(BlueprintType, Category = "Geometry")
class UROBOSIM_API USDFGeometryCylinder : public USDFGeometry
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "GeometryCylinder")
	float Radius;

	UPROPERTY(VisibleAnywhere, Category = "GeometryCylinder")
	float Length;
};

UCLASS(BlueprintType, Category = "Geometry")
class UROBOSIM_API USDFGeometrySphere : public USDFGeometry
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "GeometrySphere")
	float Radius;
};