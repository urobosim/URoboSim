
#pragma once

#include "CoreMinimal.h"
// #include "BrushBuilder.h"
// #include "Editor/UnrealEd/Classes/Builders/CubeBuilder.h"
// #include "Editor/UnrealEd/Classes/Builders/TetrahedronBuilder.h"
// #include "Editor/UnrealEd/Classes/Builders/CylinderBuilder.h"
// #include "Runtime/Engine/Classes/Engine/Brush.h"
// #include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "SDF/SDFDataAsset.h"
#include "CustomMeshComponent.h"


// #include "Editor.h"
// #include "Editor/EditorEngine.h"
// #include "EditorModeManager.h"
// #include "ActorEditorUtils.h"
#include "EngineUtils.h"
#include "LevelUtils.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Engine/Selection.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "FileHelpers.h"
// #include "RStaticMeshEditUtils.generated.h"

#include "MeshDescription.h"
#include "StaticMeshDescription.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "AssetRegistryModule.h"


class AActor;

UENUM(BlueprintType)
enum class ECollisionType : uint8
{
	None			UMETA(DisplayName = "None"),
    DopX10			UMETA(DisplayName = "10DopX"),
    DopY10			UMETA(DisplayName = "10DopY"),
    DopZ10			UMETA(DisplayName = "10DopZ"),
    DopSC18			UMETA(DisplayName = "18DopSC"),
    DopSC26			UMETA(DisplayName = "26DopSC"),
};


/*This class provides utility functions to create and edit static meshs*/

class UROBOSIMEDITOR_API RStaticMeshUtils
{
public:

    // static UStaticMesh* CreateStaticMesh(AActor* InOwner, ESDFGeometryType InGeometryTyp, TArray<float> InParameters);
	template <typename T>
    static UStaticMesh* CreateStaticMesh(UStaticMeshComponent* InOwner, T* InVisual);

    static UStaticMesh* LoadMesh(UStaticMeshComponent* InOwner, UStaticMesh* Mesh);

    static void CreateComplexCollision(UStaticMesh* OutMesh, uint32 InHullCount, int32 InMaxHullVerts, uint32 InHullPrecision);

	static void CleanUp(FString InName);
    static void GenerateKDop(UStaticMesh* OutMesh, ECollisionType InCollisionType);
    static void GenerateKDop(UStaticMesh* OutMesh, const FVector* InDirections, uint32 InNumInDirections);
    static TArray<float> GetGeometryParameter(USDFGeometry* InGeometry);

    static UStaticMesh* CreateStaticMesh(UPackage* InPackage, FString InPackageName, ESDFGeometryType InShape, TArray<float> InParameters);
protected:

    static UProceduralMeshComponent* CreateCubePM(UObject* InOwner, TArray<float> InParameter);
    static UProceduralMeshComponent* CreateCylinderPM(UObject* InOwner, TArray<float> InParameter, float Section = 8);
    static void GenerateCylinder(TArray<FVector>& InVertices, TArray<int32>& InTriangles,TArray<FVector2D>& InUVs, TArray<FVector> InNormals, TArray<FProcMeshTangent> InTangets, float Height, float InWidth, int32 InCrossSectionCount, bool bCapEnds = false, bool bDoubleSided = false, bool bInSmoothNormals = true);

};
