
#include "RStaticMeshEditUtils.h"

// #include "GameFramework/Actor.h"
// necessary for Collision creation
#include "Private/ConvexDecompTool.h"
// necessary for Collision creation KDOP
#include "Editor/UnrealEd/Private/GeomFitUtils.h"
//if commented incomplete type bodySetup
#include "Runtime/Engine/Classes/PhysicsEngine/BodySetup.h"
#include "EngineAnalytics.h"
#include "RenderingThread.h"
#include "Runtime/Analytics/Analytics/Public/Interfaces/IAnalyticsProvider.h"


#define LOCTEXT_NAMESPACE "StaticMeshEditor"
#ifndef USE_ASYNC_DECOMP
#define USE_ASYNC_DECOMP 0
#endif

// UStaticMesh* RStaticMeshUtils::CreateStaticMesh(AActor* InOwner,
//                                                 ESDFGeometryType InGeometryTyp,
//                                                 TArray<float> InParameters)
// {
//     bool bIsAdditive = true;
//     UStaticMesh* StaticMesh = nullptr;
//     UWorld *World = GEditor->GetEditorWorldContext().World();
//     ABrush* DefBrush = World->GetDefaultBrush();
//     CreateBrushBuilder(DefBrush, InGeometryTyp, InParameters);
//     if(DefBrush->BrushBuilder)
//     {
//         DefBrush->BrushBuilder->Build(World, DefBrush);
//         DefBrush->SetNeedRebuild(DefBrush->GetLevel());
//         GEditor->RebuildAlteredBSP();
//         StaticMesh = CreateStaticMeshFromBrush(InOwner, TEXT("Test"),World->GetDefaultBrush(), World->GetDefaultBrush()->Brush);
//     }
//     return StaticMesh;
// }


UStaticMesh* RStaticMeshUtils::LoadMesh(UStaticMeshComponent* InOwner, UStaticMesh* Mesh)
{
    UStaticMesh* StaticMesh = Mesh;

    return StaticMesh;
}

void RStaticMeshUtils::CreateBrushBuilder(ABrush* OutBrush,
                                          ESDFGeometryType InGeometryTyp,
                                          TArray<float> InParameters)
{
    switch(InGeometryTyp)
    {
        case ESDFGeometryType::None :
            UE_LOG(LogTemp, Error, TEXT("GeometryTyp Not Implemented"));
            break;
        case ESDFGeometryType::Mesh :
            UE_LOG(LogTemp, Error, TEXT("Mesh Not Implemented"));
            break;
        case ESDFGeometryType::Box :
            OutBrush->BrushBuilder = CreateCubeBuilder(OutBrush, InParameters);
            break;
        case ESDFGeometryType::Cylinder :
            OutBrush->BrushBuilder = CreateCylinderBuilder(OutBrush, InParameters);
            break;
        case ESDFGeometryType::Sphere :
            OutBrush->BrushBuilder = CreateSphereBuilder(OutBrush, InParameters);
            break;
        default :
            UE_LOG(LogTemp, Error, TEXT("GeometryType not supportet."));
            break;
    }
}

UCubeBuilder* RStaticMeshUtils::CreateCubeBuilder(ABrush* OutBrush,
                                                  TArray<float> InParameters)
{
    UCubeBuilder* CubeBuilder = NewObject<UCubeBuilder>(OutBrush,"CubeBuilder");
    //ClampMin 0.000001
    CubeBuilder->X = InParameters[0];
    //ClampMin 0.000001
    CubeBuilder->Y = InParameters[1];
    //ClampMin 0.000001
    CubeBuilder->Z = InParameters[2];
    return CubeBuilder;
}

UTetrahedronBuilder* RStaticMeshUtils::CreateSphereBuilder(ABrush* OutBrush, TArray<float> InParameters)
{
    UTetrahedronBuilder* SphereBuilder = NewObject<UTetrahedronBuilder>(OutBrush,"SphereBuilder");
	if(InParameters[0] < 0.1)
	{
		//ClampMin 0.000001
		SphereBuilder->Radius = 0.1;
		//ClampMax 5 ClampMin 1
		SphereBuilder->SphereExtrapolation = 1;
	}
	else
	{
		SphereBuilder->Radius = InParameters[0];
		SphereBuilder->SphereExtrapolation = 4;
	}
    return SphereBuilder;
}

UCylinderBuilder* RStaticMeshUtils::CreateCylinderBuilder(ABrush* OutBrush, TArray<float> InParameters)
{
    UCylinderBuilder* CylinderBuilder = NewObject<UCylinderBuilder>(OutBrush,"CylinderBuilder");
    CylinderBuilder->OuterRadius = InParameters[0];
    CylinderBuilder->Z = InParameters[1];
    CylinderBuilder->Sides = 8;
    CylinderBuilder->InnerRadius = 0;
    CylinderBuilder->Hollow = 0;
    CylinderBuilder->AlignToSide = 1;
    return CylinderBuilder;
}

void RStaticMeshUtils::CreateComplexCollision(UStaticMesh* OutMesh, uint32 InHullCount, int32 InMaxHullVerts, uint32 InHullPrecision)
{
	if(OutMesh && OutMesh->RenderData)
	{
		FStaticMeshLODResources& LODModel = OutMesh->RenderData->LODResources[0];

		int32 NumVerts = LODModel.VertexBuffers.StaticMeshVertexBuffer.GetNumVertices();
		TArray<FVector> Verts;

		for(int32 i=0; i<NumVerts; i++)
		{
			FVector Vert = LODModel.VertexBuffers.PositionVertexBuffer.VertexPosition(i);
			Verts.Add(Vert);
		}

		// Grab all indices
		TArray<uint32> AllIndices;
		LODModel.IndexBuffer.GetCopy(AllIndices);

		// Only copy indices that have collision enabled
		TArray<uint32> CollidingIndices;
		for(const FStaticMeshSection& Section : LODModel.Sections)
		{
			if(Section.bEnableCollision)
			{
				for (uint32 IndexIdx = Section.FirstIndex; IndexIdx < Section.FirstIndex + (Section.NumTriangles * 3); IndexIdx++)
				{
					CollidingIndices.Add(AllIndices[IndexIdx]);
				}
			}
		}

		// ClearSelectedPrims();
		FlushRenderingCommands();

		// Get the BodySetup we are going to put the collision into
		UBodySetup* bs = OutMesh->BodySetup;
		if(bs)
		{
			bs->RemoveSimpleCollision();
		}
		else
		{
			// Otherwise, create one here.
			OutMesh->CreateBodySetup();
			bs = OutMesh->BodySetup;
		}


		if(Verts.Num() >= 3 && CollidingIndices.Num() >= 3)
		{
			DecomposeMeshToHulls(bs, Verts, CollidingIndices, InHullCount, InMaxHullVerts, InHullPrecision);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("To few Verts or CollidingIndices!"));
		}

		// // Enable collision, if not already
		// if( !Viewport->GetViewportClient().IsShowSimpleCollisionChecked() )
		// {
		//	 Viewport->GetViewportClient().ToggleShowSimpleCollision();
		// }

		// refresh collision change back to staticmesh components
		RefreshCollisionChange(*OutMesh);

		OutMesh->MarkPackageDirty();
		OutMesh->bCustomizedCollision = true;	//mark the static mesh for collision customization
		// // Update screen.
		// Viewport->RefreshViewport();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Mesh or RenderData not found."));
	}
}

void RStaticMeshUtils::GenerateKDop(UStaticMesh* OutMesh, const FVector* InDirections, uint32 InNumInDirections)
{
    TArray<FVector>	DirArray;
    for(uint32 DirectionIndex = 0;DirectionIndex < InNumInDirections;DirectionIndex++)
    {
        DirArray.Add(InDirections[DirectionIndex]);
    }

    GEditor->BeginTransaction(LOCTEXT("FStaticMeshEditor_GenerateKDop", "Create Convex Collision"));
    const int32 PrimIndex = GenerateKDopAsSimpleCollision(OutMesh, DirArray);
    GEditor->EndTransaction();
    if (PrimIndex != INDEX_NONE)
    {
        if (FEngineAnalytics::IsAvailable())
        {
            FEngineAnalytics::GetProvider().RecordEvent(TEXT("Editor.Usage.StaticMesh.Collision"), TEXT("Type"), TEXT("KDop Collision"));
        }
        // const FPrimData PrimData = FPrimData(EAggCollisionShape::Convex, PrimIndex);
        // Don't 'nudge' KDop prims, as they are fitted specifically around the geometry
    }
}

void RStaticMeshUtils::GenerateKDop(UStaticMesh* OutMesh, ECollisionType InCollisionType)
{
    switch(InCollisionType)
    {
        case ECollisionType::None :
            break;
        case ECollisionType::DopX10 :
            RStaticMeshUtils::GenerateKDop(OutMesh, KDopDir10X, 10);
            break;
        case ECollisionType::DopY10 :
            RStaticMeshUtils::GenerateKDop(OutMesh, KDopDir10Y, 10);
            break;
        case ECollisionType::DopZ10 :
            RStaticMeshUtils::GenerateKDop(OutMesh, KDopDir10Z, 10);
            break;
        case ECollisionType::DopSC18 :
            RStaticMeshUtils::GenerateKDop(OutMesh, KDopDir18, 18);
            break;
        case ECollisionType::DopSC26 :
            RStaticMeshUtils::GenerateKDop(OutMesh, KDopDir26, 26);
            break;
        default :
            UE_LOG(LogTemp, Error, TEXT("Collision Type not supportet."));
            break;
    }
}


TArray<float> RStaticMeshUtils::GetGeometryParameter(USDFGeometry* InGeometry)
{
    TArray<float> Parameters;
    switch(InGeometry->Type)
    {
        case ESDFGeometryType::Box :
            Parameters.Add(InGeometry->Size.X);
            Parameters.Add(InGeometry->Size.Y);
            Parameters.Add(InGeometry->Size.Z);
            break;
        case ESDFGeometryType::Cylinder :
            Parameters.Add(InGeometry->Radius);
            Parameters.Add(InGeometry->Length);
            break;
        case ESDFGeometryType::Sphere :
            Parameters.Add(InGeometry->Radius);
            break;
        default :
            UE_LOG(LogTemp, Log, TEXT("No Parameters for Geometry"));
            break;
    }
    return Parameters;
}

void RStaticMeshUtils::CleanUp(FString InName)
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	for (TActorIterator<AStaticMeshActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		if(ActorItr->GetName().Equals(InName+"_StaticMesh"))
		{
			World->EditorDestroyActor(*ActorItr,true);
		}

	}
}
UProceduralMeshComponent* RStaticMeshUtils::CreateCubePM(UObject* InOwner, TArray<float> InParameter)
{
  UProceduralMeshComponent* ProcMeshComp = NewObject<UProceduralMeshComponent>(InOwner);
  FVector BoxRadius(InParameter[0]/ 2.0, InParameter[1]/ 2.0, InParameter[2]/ 2.0);
  TArray<FVector> Vertices;
  TArray<int32> Triangles;
  TArray<FVector> Normals;
  TArray<FVector2D> UVs;
  TArray<FProcMeshTangent> Tangent;
  TArray<FLinearColor> Colors;
  UKismetProceduralMeshLibrary::GenerateBoxMesh(BoxRadius, Vertices, Triangles, Normals, UVs, Tangent);
  for(int i = 0; i < Vertices.Num(); i++)
    {
      Colors.Add(FLinearColor::Green);
    }
  ProcMeshComp->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangent, false);
  UMaterial* Material = NewObject<UMaterial>(ProcMeshComp);
  ProcMeshComp->SetMaterial(0, Material);

  return ProcMeshComp;
}

UProceduralMeshComponent* RStaticMeshUtils::CreateCylinderPM(UObject* InOwner, TArray<float> InParameter, float Section)
{
  UProceduralMeshComponent* ProcMeshComp = NewObject<UProceduralMeshComponent>(InOwner);
  TArray<FVector> Vertices;
  TArray<int32> Triangles;
  TArray<FVector> Normals;
  TArray<FVector2D> UVs;
  TArray<FProcMeshTangent> Tangent;
  TArray<FLinearColor> Colors;


  GenerateCylinder(Vertices, Triangles, UVs, Normals, Tangent, InParameter[1], InParameter[0], 16);

  for(int i = 0; i < Vertices.Num(); i++)
    {
      Colors.Add(FLinearColor::Green);
    }

  ProcMeshComp->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangent, false);
  UMaterial* Material = NewObject<UMaterial>(ProcMeshComp);
  ProcMeshComp->SetMaterial(0, Material);

  return ProcMeshComp;
}

void RStaticMeshUtils::GenerateCylinder(TArray<FVector>& InVertices, TArray<int32>& InTriangles, TArray<FVector2D>& InUVs, TArray<FVector> InNormals, TArray<FProcMeshTangent> InTangets, float InHeight, float InWidth, int32 InCrossSectionCount, bool bInCapEnds, bool bInDoubleSided, bool bInSmoothNormals/* = true*/)
{
  // -------------------------------------------------------
  // Basic setup
  int32 VertexIndex = 0;
  int32 TriangleIndex = 0;

  // -------------------------------------------------------
  // Make a cylinder section
  const float AngleBetweenQuads = (2.0f / (float)(InCrossSectionCount)) * PI;
  const float VMapPerQuad = 1.0f / (float)InCrossSectionCount;
  FVector Offset = FVector(0, 0, InHeight);

  // Start by building up vertices that make up the cylinder sides
  //Bottom Vertex
  FVector pb = FVector(0.0f, 0.0f, -InHeight/2.0);
  //Top Vertex
  FVector pt = FVector(0.0f, 0.0f, InHeight/2.0);

  InVertices.Add(pb);
  InVertices.Add(pt);

  VertexIndex = 2;

  for (int32 QuadIndex = 0; QuadIndex < InCrossSectionCount; QuadIndex++)
    {
      float Angle = (float)QuadIndex * AngleBetweenQuads;
      float NextAngle = (float)(QuadIndex + 1) * AngleBetweenQuads;

  //     // Set up the vertices
      FVector p0 = FVector(FMath::Cos(Angle) * InWidth, FMath::Sin(Angle) * InWidth, -InHeight/2.0);
      FVector p1 = FVector(FMath::Cos(NextAngle) * InWidth, FMath::Sin(NextAngle) * InWidth, -InHeight/2.0);
      FVector p2 = p1 + Offset;
      FVector p3 = p0 + Offset;

  //     // Set up the quad triangles
      int32 VertIndex1 = VertexIndex++;
      int32 VertIndex2 = VertexIndex++;
      int32 VertIndex3 = VertexIndex++;
      int32 VertIndex4 = VertexIndex++;

      InVertices.Add(p0);
      InVertices.Add(p1);
      InVertices.Add(p2);
      InVertices.Add(p3);

  //     // Now create two triangles from those four vertices
  //     // The order of these (clockwise/counter-clockwise) dictates which way the normal will face.
      InTriangles.Add(VertIndex4);
      InTriangles.Add(VertIndex3);
      InTriangles.Add(VertIndex1);

      InTriangles.Add(VertIndex3);
      InTriangles.Add(VertIndex2);
      InTriangles.Add(VertIndex1);

      InTriangles.Add(1);
      InTriangles.Add(VertIndex3);
      InTriangles.Add(VertIndex4);

      InTriangles.Add(0);
      InTriangles.Add(VertIndex1);
      InTriangles.Add(VertIndex2);
      // UVs.  Note that Unreal UV origin (0,0) is top left
      InUVs.Add(FVector2D(1.0f - (VMapPerQuad * QuadIndex), 1.0f));
      InUVs.Add(FVector2D(1.0f - (VMapPerQuad * (QuadIndex + 1)), 1.0f));
      InUVs.Add(FVector2D(1.0f - (VMapPerQuad * (QuadIndex + 1)), 0.0f));
      InUVs.Add(FVector2D(1.0f - (VMapPerQuad * QuadIndex), 0.0f));

      // Normals
      FVector NormalCurrent = FVector::CrossProduct(p0 - p2, p1 - p2).GetSafeNormal();

      if (bInSmoothNormals)
        {
          // To smooth normals we give the vertices different values than the polygon they belong to.
          // GPUs know how to interpolate between those.
          // I do this here as an average between normals of two adjacent polygons
          float NextNextAngle = (float)(QuadIndex + 2) * AngleBetweenQuads;
          FVector p4 = FVector(FMath::Cos(NextNextAngle) * InWidth, FMath::Sin(NextNextAngle) * InWidth, 0.f);

          // p1 to p4 to p2
          FVector NormalNext = FVector::CrossProduct(p1 - p2, p4 - p2).GetSafeNormal();
          FVector AverageNormalRight = (NormalCurrent + NormalNext) / 2;
          AverageNormalRight = AverageNormalRight.GetSafeNormal();

          float PreviousAngle = (float)(QuadIndex - 1) * AngleBetweenQuads;
          FVector pMinus1 = FVector(FMath::Cos(PreviousAngle) * InWidth, FMath::Sin(PreviousAngle) * InWidth, 0.f);

          // p0 to p3 to pMinus1
          FVector NormalPrevious = FVector::CrossProduct(p0 - pMinus1, p3 - pMinus1).GetSafeNormal();
          FVector AverageNormalLeft = (NormalCurrent + NormalPrevious) / 2;
          AverageNormalLeft = AverageNormalLeft.GetSafeNormal();

          InNormals.Add(AverageNormalLeft);
          InNormals.Add(AverageNormalRight);
          InNormals.Add(AverageNormalRight);
          InNormals.Add(AverageNormalLeft);
        }
      else
        {
          // If not smoothing we just set the vertex normal to the same normal as the polygon they belong to
          // InVertices[VertIndex1].Normal = InVertices[VertIndex2].Normal = InVertices[VertIndex3].Normal = InVertices[VertIndex4].Normal = FPackedNormal(NormalCurrent);
          InNormals.Add(NormalCurrent);
          InNormals.Add(NormalCurrent);
          InNormals.Add(NormalCurrent);
          InNormals.Add(NormalCurrent);
        }

      // Tangents (perpendicular to the surface)
      FVector SurfaceTangent = p0 - p1;
      SurfaceTangent = SurfaceTangent.GetSafeNormal();
      InTangets.Add(FProcMeshTangent(SurfaceTangent, false));
      InTangets.Add(FProcMeshTangent(SurfaceTangent, false));
      InTangets.Add(FProcMeshTangent(SurfaceTangent, false));
      InTangets.Add(FProcMeshTangent(SurfaceTangent, false));

      // -------------------------------------------------------
  //     // If double sided, create extra polygons but face the normals the other way.
  //     if (bInDoubleSided)
  //       {
  //         VertIndex1 = VertexIndex++;
  //         VertIndex2 = VertexIndex++;
  //         VertIndex3 = VertexIndex++;
  //         VertIndex4 = VertexIndex++;

  //         InVertices[VertIndex1].Position = p0;
  //         InVertices[VertIndex2].Position = p1;
  //         InVertices[VertIndex3].Position = p2;
  //         InVertices[VertIndex4].Position = p3;

  //         // Reverse the poly order to face them the other way
  //         InTriangles[TriangleIndex++] = VertIndex4;
  //         InTriangles[TriangleIndex++] = VertIndex1;
  //         InTriangles[TriangleIndex++] = VertIndex3;

  //         InTriangles[TriangleIndex++] = VertIndex3;
  //         InTriangles[TriangleIndex++] = VertIndex1;
  //         InTriangles[TriangleIndex++] = VertIndex2;

  //         // UVs  (Unreal 1,1 is top left)
  //         InVertices[VertIndex1].UV0 = FVector2D(1.0f - (VMapPerQuad * QuadIndex), 1.0f);
  //         InVertices[VertIndex2].UV0 = FVector2D(1.0f - (VMapPerQuad * (QuadIndex + 1)), 1.0f);
  //         InVertices[VertIndex3].UV0 = FVector2D(1.0f - (VMapPerQuad * (QuadIndex + 1)), 0.0f);
  //         InVertices[VertIndex4].UV0 = FVector2D(1.0f - (VMapPerQuad * QuadIndex), 0.0f);

  //         // Just simple (unsmoothed) normal for these
  //         InVertices[VertIndex1].Normal = InVertices[VertIndex2].Normal = InVertices[VertIndex3].Normal = InVertices[VertIndex4].Normal = FPackedNormal(NormalCurrent);

  //         // Tangents (perpendicular to the surface)
  //         FVector SurfaceTangentDbl = p0 - p1;
  //         SurfaceTangentDbl = SurfaceTangentDbl.GetSafeNormal();
  //         InVertices[VertIndex1].Tangent = InVertices[VertIndex2].Tangent = InVertices[VertIndex3].Tangent = InVertices[VertIndex4].Tangent = FPackedNormal(SurfaceTangentDbl);
  //       }

      // -------------------------------------------------------
      // Caps are closed here by triangles that start at 0, then use the points along the circle for the other two corners.
      // A better looking method uses a vertex in the center of the circle, but uses two more polygons.  We will demonstrate that in a different sample.
  //     if (QuadIndex != 0 && QuadIndex != InCrossSectionCount - 1 && bInCapEnds)
  //       {
  //         // Bottom cap
  //         FVector capVertex0 = FVector(FMath::Cos(0) * InWidth, FMath::Sin(0) * InWidth, 0.f);
  //         FVector capVertex1 = FVector(FMath::Cos(Angle) * InWidth, FMath::Sin(Angle) * InWidth, 0.f);
  //         FVector capVertex2 = FVector(FMath::Cos(NextAngle) * InWidth, FMath::Sin(NextAngle) * InWidth, 0.f);

  //         VertIndex1 = VertexIndex++;
  //         VertIndex2 = VertexIndex++;
  //         VertIndex3 = VertexIndex++;
  //         InVertices[VertIndex1].Position = capVertex0;
  //         InVertices[VertIndex2].Position = capVertex1;
  //         InVertices[VertIndex3].Position = capVertex2;

  //         InTriangles[TriangleIndex++] = VertIndex1;
  //         InTriangles[TriangleIndex++] = VertIndex2;
  //         InTriangles[TriangleIndex++] = VertIndex3;

  //         InVertices[VertIndex1].UV0 = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
  //         InVertices[VertIndex2].UV0 = FVector2D(0.5f - (FMath::Cos(-Angle) / 2.0f), 0.5f - (FMath::Sin(-Angle) / 2.0f));
  //         InVertices[VertIndex3].UV0 = FVector2D(0.5f - (FMath::Cos(-NextAngle) / 2.0f), 0.5f - (FMath::Sin(-NextAngle) / 2.0f));

  //         FVector CapNormalCurrent = FVector::CrossProduct(InVertices[VertIndex1].Position - InVertices[VertIndex3].Position, InVertices[VertIndex2].Position - InVertices[VertIndex3].Position).GetSafeNormal();
  //         InVertices[VertIndex1].Normal = InVertices[VertIndex2].Normal = InVertices[VertIndex3].Normal = FPackedNormal(CapNormalCurrent);

  //         // Tangents (perpendicular to the surface)
  //         FVector SurfaceTangentCap = p0 - p1;
  //         SurfaceTangentCap = SurfaceTangentCap.GetSafeNormal();
  //         InVertices[VertIndex1].Tangent = InVertices[VertIndex2].Tangent = InVertices[VertIndex3].Tangent = FPackedNormal(SurfaceTangentCap);

  //         // Top cap
  //         capVertex0 = capVertex0 + Offset;
  //         capVertex1 = capVertex1 + Offset;
  //         capVertex2 = capVertex2 + Offset;

  //         VertIndex1 = VertexIndex++;
  //         VertIndex2 = VertexIndex++;
  //         VertIndex3 = VertexIndex++;
  //         InVertices[VertIndex1].Position = capVertex0;
  //         InVertices[VertIndex2].Position = capVertex1;
  //         InVertices[VertIndex3].Position = capVertex2;

  //         InTriangles[TriangleIndex++] = VertIndex3;
  //         InTriangles[TriangleIndex++] = VertIndex2;
  //         InTriangles[TriangleIndex++] = VertIndex1;

  //         InVertices[VertIndex1].UV0 = FVector2D(0.5f - (FMath::Cos(0) / 2.0f), 0.5f - (FMath::Sin(0) / 2.0f));
  //         InVertices[VertIndex2].UV0 = FVector2D(0.5f - (FMath::Cos(Angle) / 2.0f), 0.5f - (FMath::Sin(Angle) / 2.0f));
  //         InVertices[VertIndex3].UV0 = FVector2D(0.5f - (FMath::Cos(NextAngle) / 2.0f), 0.5f - (FMath::Sin(NextAngle) / 2.0f));

  //         CapNormalCurrent = FVector::CrossProduct(InVertices[VertIndex1].Position - InVertices[VertIndex3].Position, InVertices[VertIndex2].Position - InVertices[VertIndex3].Position).GetSafeNormal();
  //         InVertices[VertIndex1].Normal = InVertices[VertIndex2].Normal = InVertices[VertIndex3].Normal = FPackedNormal(CapNormalCurrent);

  //         // Tangents (perpendicular to the surface)
  //         SurfaceTangentCap = p0 - p1;
  //         SurfaceTangentCap = SurfaceTangentCap.GetSafeNormal();
  //         InVertices[VertIndex1].Tangent = InVertices[VertIndex2].Tangent = InVertices[VertIndex3].Tangent = FPackedNormal(SurfaceTangentCap);
  //       }
    }
}

#undef LOCTEXT_NAMESPACE
// #undef USE_ASYNC_DECOMP
