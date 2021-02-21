
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

	template <typename T>
    static UStaticMesh* CreateThroughBrushOrLoadMesh(UStaticMeshComponent* InOwner, T* InVisual);
	template <typename T>
    static UStaticMesh* CreateStaticMeshThroughBrush(UStaticMeshComponent* InOwner, T* InVisual);

	template <typename T>
    static UStaticMesh* CreateStaticMeshThroughBrush(USDFLink* InOwner, T* InVisual);

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

template <typename T>
UStaticMesh* RStaticMeshUtils::CreateStaticMesh(UStaticMeshComponent* InOwner, T* InVisual)
{
    return CreateThroughBrushOrLoadMesh(InOwner, InVisual);
};

template <typename T>
UStaticMesh* RStaticMeshUtils::CreateThroughBrushOrLoadMesh(UStaticMeshComponent* InOwner, T* InVisual)
{
    UStaticMesh* StaticMesh = nullptr;
    switch(InVisual->Geometry->Type)
    {
        case ESDFGeometryType::None :
            // UE_LOG(LogTemp, Log, TEXT("GeometryTyp Not Implemented"));
            break;
        case ESDFGeometryType::Mesh :
            StaticMesh = LoadMesh(InOwner, InVisual->Geometry->Mesh);
            break;
        case ESDFGeometryType::Box :
            StaticMesh = CreateStaticMeshThroughBrush(InOwner, InVisual);
            break;
        case ESDFGeometryType::Cylinder :
            StaticMesh = CreateStaticMeshThroughBrush(InOwner, InVisual);
            break;
        case ESDFGeometryType::Sphere :
            StaticMesh = CreateStaticMeshThroughBrush(InOwner, InVisual);
            break;
        default :
            UE_LOG(LogTemp, Error, TEXT("GeometryType not supportet."));
            break;
    }
    return StaticMesh;
};

template <typename T>
UStaticMesh* RStaticMeshUtils::CreateStaticMeshThroughBrush(UStaticMeshComponent* InOwner, T* InVisual)
{

  UStaticMesh* StaticMesh = nullptr;
  FString PackageName = "";
  FString Reason = "";
  // FString NewDir = DataAsset->GetOuter()->GetPathName() + "/" + CurrentLinkName;
  if(!FPackageName::TryConvertFilenameToLongPackageName("/Game/Meshes/" + InVisual->Name + "_StaticMesh", PackageName, &Reason))
    {
      UE_LOG(LogTemp, Error, TEXT("Packacke name invlaide because : %s"), *Reason);
      return nullptr;
    }
  StaticMesh = LoadObject<UStaticMesh>(InOwner, *PackageName, NULL, LOAD_None, NULL);
  if(StaticMesh)
    {
      return StaticMesh;
    }

  UProceduralMeshComponent* ProcMeshComp = nullptr;

  switch(InVisual->Geometry->Type)
    {
        case ESDFGeometryType::None :
            UE_LOG(LogTemp, Error, TEXT("GeometryTyp Not Implemented"));
            break;
        case ESDFGeometryType::Mesh :
            UE_LOG(LogTemp, Error, TEXT("Mesh Not Implemented"));
            break;
        case ESDFGeometryType::Box :
          ProcMeshComp = CreateCubePM(InOwner, GetGeometryParameter(InVisual->Geometry));
            break;
        case ESDFGeometryType::Cylinder :
          ProcMeshComp = CreateCylinderPM(InOwner, GetGeometryParameter(InVisual->Geometry));
            break;
        case ESDFGeometryType::Sphere :
            UE_LOG(LogTemp, Error, TEXT("GeometryType not supportet."));
            break;
        default :
            UE_LOG(LogTemp, Error, TEXT("GeometryType not supportet."));
            break;
    }
  // FVector BoxRadius(1, 1, 1);
  // TArray<FVector> Vertices;
  // TArray<int32> Triangles;
  // TArray<FVector> Normals;
  // TArray<FVector2D> UVs;
  // TArray<FProcMeshTangent> Tangent;
  // TArray<FLinearColor> Colors;
  // UKismetProceduralMeshLibrary::GenerateBoxMesh(BoxRadius, Vertices, Triangles, Normals, UVs, Tangent);
  // for(int i = 0; i < Vertices.Num(); i++)
  //   {
  //     Colors.Add(FLinearColor::Green);
  //   }
  // ProcMeshComp->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangent, false);
  // UMaterial* Material = NewObject<UMaterial>(ProcMeshComp);
  // ProcMeshComp->SetMaterial(0, Material);




  FString Name;
  FName MeshName(*FPackageName::GetLongPackageAssetName(PackageName));
  // UStaticMesh* StaticMesh = NewObject<UStaticMesh>(InOwner, MeshName, RF_Public | RF_Standalone);
  if (ProcMeshComp != nullptr)
    {
      // Get the full name of where we want to create the physics asset.

      FMeshDescription MeshDescription;
      UStaticMesh::RegisterMeshAttributes(MeshDescription);
      FStaticMeshDescriptionAttributeGetter AttributeGetter(&MeshDescription);
      TPolygonGroupAttributesRef<FName> PolygonGroupNames = AttributeGetter.GetPolygonGroupImportedMaterialSlotNames();
      TVertexAttributesRef<FVector> VertexPositions = AttributeGetter.GetPositions();
      TVertexInstanceAttributesRef<FVector> Tangents = AttributeGetter.GetTangents();
      TVertexInstanceAttributesRef<float> BinormalSigns = AttributeGetter.GetBinormalSigns();
      TVertexInstanceAttributesRef<FVector> Normals = AttributeGetter.GetNormals();
      TVertexInstanceAttributesRef<FVector4> Colors = AttributeGetter.GetColors();
      TVertexInstanceAttributesRef<FVector2D> UVs = AttributeGetter.GetUVs();
      TEdgeAttributesRef<bool> EdgeHardnesses = AttributeGetter.GetEdgeHardnesses();
      TEdgeAttributesRef<float> EdgeCreaseSharpnesses = AttributeGetter.GetEdgeCreaseSharpnesses();

      // Materials to apply to new mesh
      const int32 NumSections = ProcMeshComp->GetNumSections();
      int32 VertexCount = 0;
      int32 VertexInstanceCount = 0;
      int32 PolygonCount = 0;
      TMap<UMaterialInterface*, FPolygonGroupID> UniqueMaterials;
      UniqueMaterials.Reserve(NumSections);
      TArray<FPolygonGroupID> MaterialRemap;
      MaterialRemap.Reserve(NumSections);
      //Get all the info we need to create the MeshDescription
      for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
        {
          FProcMeshSection* ProcSection = ProcMeshComp->GetProcMeshSection(SectionIdx);
          VertexCount += ProcSection->ProcVertexBuffer.Num();
          VertexInstanceCount += ProcSection->ProcIndexBuffer.Num();
          PolygonCount += ProcSection->ProcIndexBuffer.Num() / 3;
          UMaterialInterface*Material = ProcMeshComp->GetMaterial(SectionIdx);
          if (!UniqueMaterials.Contains(Material))
            {
              FPolygonGroupID NewPolygonGroup = MeshDescription.CreatePolygonGroup();
              UniqueMaterials.Add(Material, NewPolygonGroup);
              PolygonGroupNames[NewPolygonGroup] = Material->GetFName();
            }
          FPolygonGroupID* PolygonGroupID = UniqueMaterials.Find(Material);
          check(PolygonGroupID != nullptr);
          MaterialRemap.Add(*PolygonGroupID);
        }
      MeshDescription.ReserveNewVertices(VertexCount);
      MeshDescription.ReserveNewVertexInstances(VertexInstanceCount);
      MeshDescription.ReserveNewPolygons(PolygonCount);
      MeshDescription.ReserveNewEdges(PolygonCount * 2);
      UVs.SetNumIndices(4);
      //Add Vertex and VertexInstance and polygon for each section
      for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
        {
          FProcMeshSection* ProcSection = ProcMeshComp->GetProcMeshSection(SectionIdx);
          FPolygonGroupID PolygonGroupID = MaterialRemap[SectionIdx];
          //Create the vertex
          int32 NumVertex = ProcSection->ProcVertexBuffer.Num();
          TMap<int32, FVertexID> VertexIndexToVertexID;
          VertexIndexToVertexID.Reserve(NumVertex);
          for (int32 VertexIndex = 0; VertexIndex < NumVertex; ++VertexIndex)
            {
              FProcMeshVertex& Vert = ProcSection->ProcVertexBuffer[VertexIndex];
              const FVertexID VertexID = MeshDescription.CreateVertex();
              VertexPositions[VertexID] = Vert.Position;
              VertexIndexToVertexID.Add(VertexIndex, VertexID);
            }
          //Create the VertexInstance
          int32 NumIndices = ProcSection->ProcIndexBuffer.Num();
          int32 NumTri = NumIndices / 3;
          TMap<int32, FVertexInstanceID> IndiceIndexToVertexInstanceID;
          IndiceIndexToVertexInstanceID.Reserve(NumVertex);
          for (int32 IndiceIndex = 0; IndiceIndex < NumIndices; IndiceIndex++)
            {
              const int32 VertexIndex = ProcSection->ProcIndexBuffer[IndiceIndex];
              const FVertexID VertexID = VertexIndexToVertexID[VertexIndex];
              const FVertexInstanceID VertexInstanceID = MeshDescription.CreateVertexInstance(VertexID);
              IndiceIndexToVertexInstanceID.Add(IndiceIndex, VertexInstanceID);

              FProcMeshVertex& ProcVertex = ProcSection->ProcVertexBuffer[VertexIndex];

              Tangents[VertexInstanceID] = ProcVertex.Tangent.TangentX;
              Normals[VertexInstanceID] = ProcVertex.Normal;
              BinormalSigns[VertexInstanceID] = ProcVertex.Tangent.bFlipTangentY ? -1.f : 1.f;

              Colors[VertexInstanceID] = FLinearColor(ProcVertex.Color);

              UVs.Set(VertexInstanceID, 0, ProcVertex.UV0);
              UVs.Set(VertexInstanceID, 1, ProcVertex.UV1);
              UVs.Set(VertexInstanceID, 2, ProcVertex.UV2);
              UVs.Set(VertexInstanceID, 3, ProcVertex.UV3);
            }

          //Create the polygons for this section
          for (int32 TriIdx = 0; TriIdx < NumTri; TriIdx++)
            {
              FVertexID VertexIndexes[3];
              TArray<FVertexInstanceID> VertexInstanceIDs;
              VertexInstanceIDs.SetNum(3);

              for (int32 CornerIndex = 0; CornerIndex < 3; ++CornerIndex)
                {
                  const int32 IndiceIndex = (TriIdx * 3) + CornerIndex;
                  const int32 VertexIndex = ProcSection->ProcIndexBuffer[IndiceIndex];
                  VertexIndexes[CornerIndex] = VertexIndexToVertexID[VertexIndex];
                  VertexInstanceIDs[CornerIndex] = IndiceIndexToVertexInstanceID[IndiceIndex];
                }

              // Insert a polygon into the mesh
              const FPolygonID NewPolygonID = MeshDescription.CreatePolygon(PolygonGroupID, VertexInstanceIDs);
              //Triangulate the polygon
              FMeshPolygon& Polygon = MeshDescription.GetPolygon(NewPolygonID);
              MeshDescription.ComputePolygonTriangulation(NewPolygonID, Polygon.Triangles);
            }
        }

      // If we got some valid data.
      if (MeshDescription.Polygons().Num() > 0)
        {
          // Then find/create it.
          UPackage* Package = CreatePackage(NULL, *PackageName);
          check(Package);

          // Create StaticMesh object
          StaticMesh = NewObject<UStaticMesh>(Package, MeshName, RF_Public | RF_Standalone);
          StaticMesh->InitResources();

          StaticMesh->LightingGuid = FGuid::NewGuid();

          // Add source to new StaticMesh
          FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
          SrcModel.BuildSettings.bRecomputeNormals = false;
          SrcModel.BuildSettings.bRecomputeTangents = false;
          SrcModel.BuildSettings.bRemoveDegenerates = false;
          SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
          SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
          SrcModel.BuildSettings.bGenerateLightmapUVs = true;
          SrcModel.BuildSettings.SrcLightmapIndex = 0;
          SrcModel.BuildSettings.DstLightmapIndex = 1;
          FMeshDescription* OriginalMeshDescription = StaticMesh->GetMeshDescription(0);
          if (OriginalMeshDescription == nullptr)
            {
              OriginalMeshDescription = StaticMesh->CreateMeshDescription(0);
            }
          *OriginalMeshDescription = MeshDescription;
          StaticMesh->CommitMeshDescription(0);

          // Copy materials to new mesh
          for (auto Kvp : UniqueMaterials)
            {
              UMaterialInterface* Material = Kvp.Key;
              StaticMesh->StaticMaterials.Add(FStaticMaterial(Material, Material->GetFName(), Material->GetFName()));
            }

          //Set the Imported version before calling the build
          StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;

          // Build mesh from source
          StaticMesh->Build(false);
          StaticMesh->PostEditChange();

          // Notify asset registry of new asset
          FAssetRegistryModule::AssetCreated(StaticMesh);
        }

    }
  return StaticMesh;
}


template <typename T>
UStaticMesh* RStaticMeshUtils::CreateStaticMeshThroughBrush(USDFLink* InOwner, T* InVisual)
{
};
