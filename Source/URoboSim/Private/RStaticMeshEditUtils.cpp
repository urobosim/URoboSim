
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

#undef LOCTEXT_NAMESPACE
// #undef USE_ASYNC_DECOMP
