
#pragma once

#include "CoreMinimal.h"
// #include "BrushBuilder.h"
#include "Editor/UnrealEd/Classes/Builders/CubeBuilder.h"
#include "Editor/UnrealEd/Classes/Builders/TetrahedronBuilder.h"
#include "Editor/UnrealEd/Classes/Builders/CylinderBuilder.h"
// #include "Runtime/Engine/Classes/Engine/Brush.h"
// #include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "SDF/SDFDataAsset.h"

// #include "Editor.h"
#include "Editor/EditorEngine.h"
#include "EditorModeManager.h"
#include "ActorEditorUtils.h"
#include "EngineUtils.h"
#include "LevelUtils.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Engine/Selection.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"
#include "FileHelpers.h"
// #include "RStaticMeshEditUtils.generated.h"

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

class UROBOSIM_API RStaticMeshUtils
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
protected:

    static void CreateBrushBuilder(ABrush* OutBrush, ESDFGeometryType InGeometryTyp, TArray<float> InParameters);

    static UCubeBuilder* CreateCubeBuilder(ABrush* OutBrush, TArray<float> InParameters);

    static UTetrahedronBuilder* CreateSphereBuilder(ABrush* OutBrush, TArray<float> InParameters);

    static UCylinderBuilder* CreateCylinderBuilder(ABrush* OutBrush, TArray<float> InParameters);

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
	if(!FPackageName::TryConvertFilenameToLongPackageName("/Game/" + InVisual->Name + "_StaticMesh", PackageName, &Reason))
	{
		UE_LOG(LogTemp, Error, TEXT("Packacke name invlaide because : %s"), *Reason);
		return nullptr;
	}

	FString MeshReference = TEXT("StaticMesh\'") + PackageName + "." + FPackageName::GetShortName(PackageName)+ "\'";
	StaticMesh = LoadObject<UStaticMesh>(NULL, *MeshReference, NULL, LOAD_None, NULL);
	if(!StaticMesh)
	{
		UWorld *World = GEditor->GetEditorWorldContext().World();
		if ((World != nullptr) && (World->GetDefaultBrush() != nullptr))
		{
			ULevel* CurrentLevel = World->GetCurrentLevel();
			ABrush* DefBrush = World->GetDefaultBrush();
			TArray<float> Parameters = GetGeometryParameter(InVisual->Geometry);
			CreateBrushBuilder(DefBrush, InVisual->Geometry->Type, Parameters);
			if (CurrentLevel != nullptr && !FLevelUtils::IsLevelLocked(CurrentLevel)
					&& DefBrush->BrushBuilder)
			{
				World->GetDefaultBrush()->BrushBuilder = DuplicateObject<UBrushBuilder>(DefBrush->BrushBuilder, World->GetDefaultBrush()->GetOuter());
				World->GetDefaultBrush()->BrushBuilder->Build(World, DefBrush);
				DefBrush->SetNeedRebuild(CurrentLevel);
				GEditor->RebuildAlteredBSP();
				bool bIsAdditive = true;
				const TCHAR* Command = bIsAdditive ? TEXT("BRUSH ADD SELECTNEWBRUSH") : TEXT("BRUSH SUBTRACT SELECTNEWBRUSH");
				GEditor->Exec(World, Command);
				USelection* SelectedActor = GEditor->GetSelectedActors();
				if(Cast<ABrush>(SelectedActor->GetSelectedObject(0)))
				{
					int32 BrushIndexForReattachment = 0;
					TArray<AActor* > BrushList ;
					BrushList.Add(Cast<AActor>(SelectedActor->GetSelectedObject(0)));
					GEditor->DoConvertActors(BrushList, AStaticMeshActor::StaticClass(), TSet<FString>(), false, PackageName);
				}

			}

		}
	}
	else
	{
		CleanUp(InVisual->Name);
	}
	// Early out if actor deletion is currently forbidden
	if (GEditor->ShouldAbortActorDeletion())
	{
		return nullptr;
	}

	return StaticMesh;
}

// template <typename T>
// UStaticMesh* RStaticMeshUtils::CreateStaticMeshThroughBrush(UStaticMeshComponent* InOwner, T* InVisual)
// {
//
//     UStaticMesh* StaticMesh = nullptr;
//
// 	FString PackageName = "";
// 	FString Reason = "";
// 	if(!FPackageName::TryConvertFilenameToLongPackageName("/Game/" + InVisual->Name + "_StaticMesh", PackageName, &Reason))
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Packacke name invlaide because : %s"), *Reason);
// 		return nullptr;
// 	}
//
// 	if(!FPackageName::DoesPackageExist(PackageName))
// 	{
// 		UWorld *World = GEditor->GetEditorWorldContext().World();
// 		if ((World != nullptr) && (World->GetDefaultBrush() != nullptr))
// 		{
// 			UE_LOG(LogTemp, Warning, TEXT("1"));
// 			ULevel* CurrentLevel = World->GetCurrentLevel();
// 			ABrush* DefBrush = World->GetDefaultBrush();
// 			TArray<float> Parameters = GetGeometryParameter(InVisual->Geometry);
// 			CreateBrushBuilder(DefBrush, InVisual->Geometry->Type, Parameters);
//
// 			if (CurrentLevel != nullptr && !FLevelUtils::IsLevelLocked(CurrentLevel)
// 					&& DefBrush->BrushBuilder)
// 			{
// 				UE_LOG(LogTemp, Warning, TEXT("2"));
// 				World->GetDefaultBrush()->BrushBuilder = DuplicateObject<UBrushBuilder>(DefBrush->BrushBuilder, World->GetDefaultBrush()->GetOuter());
// 				World->GetDefaultBrush()->BrushBuilder->Build(World, DefBrush);
// 				DefBrush->SetNeedRebuild(CurrentLevel);
// 				GEditor->RebuildAlteredBSP();
// 				bool bIsAdditive = true;
// 				const TCHAR* Command = bIsAdditive ? TEXT("BRUSH ADD SELECTNEWBRUSH") : TEXT("BRUSH SUBTRACT SELECTNEWBRUSH");
// 				GEditor->Exec(World, Command);
// 				UE_LOG(LogTemp, Warning, TEXT("3"));
// 				if( !GIsTransacting )
// 				{
// 					int32 ActorCount = GEditor->GetSelectedActorCount();
// 					USelection* SelectedActor = GEditor->GetSelectedActors();
// 					for(int32 ActIdx =0; ActIdx < ActorCount; ActIdx++)
// 					{
// 						UE_LOG(LogTemp, Warning, TEXT("4"));
// 						if(ABrush* Brush = Cast<ABrush>(SelectedActor->GetSelectedObject(ActIdx)))
// 						{
// 							UE_LOG(LogTemp, Warning, TEXT("5"));
// 							if (!FActorEditorUtils::IsABuilderBrush(Brush))
// 							{
// 								UE_LOG(LogTemp, Warning, TEXT("save %s"), *InVisual->Name);
// 								TArray< TWeakObjectPtr<ABrush> > SelectedBrushes;
// 								SelectedBrushes.Add(Brush);
// 								TArray<AActor*> ValidSelectedBrushes;
// 								CopyFromWeakArray(ValidSelectedBrushes, SelectedBrushes);
//
// 								GEditor->DoConvertActors(ValidSelectedBrushes, AStaticMeshActor::StaticClass(), TSet<FString>(), true, PackageName);
// 								UEditorLoadingAndSavingUtils::SaveDirtyPackages(false, true);
// 								UE_LOG(LogTemp, Warning, TEXT("save finished"));
// // StaticMesh'/Game/fr_caster_r_wheel_link_geom_StaticMesh.fr_caster_r_wheel_link_geom_StaticMesh'
// 							}
// 							else
// 							{
// 								UE_LOG(LogTemp, Error, TEXT("Brush is a  BuilderBrush."));
// 							}
// 						}
// 						else
// 						{
// 							UE_LOG(LogTemp, Error, TEXT("Selected Actor no Brush"));
// 						}
// 					}
// 				}
// 				else
// 				{
// 					UE_LOG(LogTemp, Error, TEXT("Editor is Transacting"));
// 				}
// 			}
// 			else
// 			{
// 				UE_LOG(LogTemp, Error, TEXT("No Level or LevelLocked or no BrushBuilder."));
// 			}
// 		}
// 		else
// 		{
// 			UE_LOG(LogTemp, Error, TEXT("No world or DefaultBrushBuilder"));
// 		}
// 	}
// 	else
// 	{
//
// 		UE_LOG(LogTemp, Warning, TEXT("before CleanUp"));
// 		CleanUp(InVisual->Name);
// 		UE_LOG(LogTemp, Warning, TEXT("after CleanUp"));
// 	}
//
// 	FString MeshReference = TEXT("StaticMesh\'") + PackageName + "." + FPackageName::GetShortName(PackageName)+ "\'";
// 	StaticMesh = LoadObject<UStaticMesh>(NULL, *MeshReference, NULL, LOAD_None, NULL);
// 	if(!StaticMesh)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("Package %s not Found"), *PackageName);
// 	}
// 	UE_LOG(LogTemp, Warning, TEXT("return mesh"));
//     return StaticMesh;
// };

template <typename T>
UStaticMesh* RStaticMeshUtils::CreateStaticMeshThroughBrush(USDFLink* InOwner, T* InVisual)
{
};
