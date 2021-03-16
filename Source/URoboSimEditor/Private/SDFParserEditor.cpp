// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "SDFParserEditor.h"
#include "AssetRegistryModule.h"
#include "Paths.h"
#include "RStaticMeshEditUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogSDFParserEditor, Log, All);

// Default constructor
FSDFParserEditor::FSDFParserEditor() : AssetRegistryModule(FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry")))
{
	XmlFile = nullptr;
	bSDFLoaded = false;
}

// Constructor with load from path
FSDFParserEditor::FSDFParserEditor(const FString &InFilename) : FSDFParserEditor::FSDFParserEditor()
{
	LoadSDF(InFilename);
}

// Destructor
FSDFParserEditor::~FSDFParserEditor()
{
	Clear();
}

// Clear parser
void FSDFParserEditor::Clear()
{
	ISDFParserInterface::Clear();
	FbxFactory = nullptr;
	DirPath = TEXT("");
}

// Load sdf from file
const bool FSDFParserEditor::LoadSDF(const FString &InFilename)
{
	// Make sure parser is clean
	Clear();

	// Load xml file
	XmlFile = new FXmlFile(InFilename);

	// Check for valid sdf
	bSDFLoaded = IsValidSDF();

	// Set the model directory path
	SetDirectoryPath(InFilename);

	// Create fbx factory for loading the meshes
	FbxFactory = NewObject<UFbxFactory>(UFbxFactory::StaticClass());

	// If options are to be offered before loading.
	FbxFactory->EnableShowOption();

	return bSDFLoaded;
}

// Check if sdf data is valid
const bool FSDFParserEditor::IsValidSDF()
{
	if (XmlFile == nullptr)
	{
		return false;
	}

	// Check if root node is <sdf> or <gazebo> (sdf version 1.2)
	if (!XmlFile->GetRootNode()->GetTag().Equals(TEXT("sdf")) && !XmlFile->GetRootNode()->GetTag().Equals(TEXT("gazebo")))
	{
		UE_LOG(LogSDFParserEditor, Error, TEXT("Root node is not <sdf> or <gazebo>(sdf version 1.2)"));
		return false;
	}
	else
	{
		return true;
	}
}

// Create data asset and parse sdf data into it
USDFDataAsset *FSDFParserEditor::ParseToDataAsset(UObject *&InParent, FName &InName, EObjectFlags &InFlags)
{
	if (!bSDFLoaded)
	{
		return nullptr;
	}
	else
	{
		// Create a new SDFDataAsset
		DataAsset = NewObject<USDFDataAsset>(InParent, InName, InFlags);

		// Parse sdf data and fill the data asset
		ParseSDF();

		// Create and parse meshes
		CreateMeshes();

		return DataAsset;
	}
}

// Fix file path
void FSDFParserEditor::SetDirectoryPath(const FString &InFilename)
{
	// Replace back slash to forward slash in the path
	// D:\Models\MyModelName\model.sdf --> D:/Models/MyModelName/model.sdf
	DirPath = InFilename.Replace(TEXT("\\"), TEXT("/"));

	// Remove filename from path --> D:/Models/MyModelName
	DirPath = FPaths::GetPath(DirPath);

	// TODO rather cut uppermost from URI
	// One dir up --> D:/Models
	DirPath = FPaths::Combine(DirPath, TEXT("/.."));
	FPaths::CollapseRelativeDirectories(DirPath);
}

/* Begin helper functions */
// Create mesh
void FSDFParserEditor::CreateMeshes()
{
	for (const auto &Model : DataAsset->Models)
	{
		for (const auto &Link : Model->Links)
		{
			for (const auto &Visual : Link->Visuals)
			{
				CreateMesh(Visual->GetName(), Link->GetName(), Visual->Geometry);
			}
			for (const auto &Collision : Link->Collisions)
			{
				CreateMesh(Collision->GetName(), Link->GetName(), Collision->Geometry);
				CreateCollisionForMesh(Collision->Geometry->Mesh, Collision->Geometry->Type);
			}
		}
	}
}

// Get mesh absolute path
FString FSDFParserEditor::GetMeshAbsolutePath(const FString &Uri)
{
	// Create mesh relative path, add .fbx extension
	FString MeshRelativePath = Uri;
	if (!MeshRelativePath.EndsWith(".fbx"))
	{
		MeshRelativePath = FPaths::ChangeExtension(MeshRelativePath, TEXT("fbx"));
	}
	// Remove package name prefix
	MeshRelativePath.RemoveFromStart(TEXT("model:/"));

	// Create mesh absolute path
	return DirPath + MeshRelativePath;
}

// Create mesh
void FSDFParserEditor::CreateMesh(const FString &InMeshName, const FString &LinkName, USDFGeometry *OutGeometry)
{
	FString PackageName = CreatePackageName(InMeshName, LinkName);
	UPackage *Package = CreatePackage(nullptr, *PackageName);
	if (OutGeometry->Type == ESDFGeometryType::Mesh)
	{
		OutGeometry->Mesh = ImportMesh(Cast<USDFGeometryMesh>(OutGeometry)->Uri, InMeshName, Package);
	}
	else
	{
		OutGeometry->Mesh = RStaticMeshUtils::CreateStaticMesh(Package, PackageName, OutGeometry);
	}
}

// Create package name
FString FSDFParserEditor::CreatePackageName(const FString &InMeshName, const FString &LinkName)
{
	FString PackageName = "";
	FString Reason = "";
	FString NewDir = DataAsset->GetOuter()->GetPathName() + "/" + LinkName;
	if (!FPackageName::TryConvertFilenameToLongPackageName(NewDir + "/" + TEXT("SM_") + InMeshName + TEXT("_V"), PackageName, &Reason))
	{
		UE_LOG(LogSDFParserEditor, Error, TEXT("Package name invalid because : %s"), *Reason);
	}
	AssetRegistryModule.Get().AddPath(NewDir);
	return PackageName;
}

// Import .fbx meshes from data asset
UStaticMesh *FSDFParserEditor::ImportMesh(const FString &Uri, const FString &InMeshName, UPackage *InPackage)
{
	FString MeshAbsolutePath = GetMeshAbsolutePath(Uri);
	if (!FPaths::FileExists(MeshAbsolutePath))
	{
		UE_LOG(LogSDFParserEditor, Error, TEXT("[%s] Could not find %s"), *FString(__FUNCTION__), *MeshAbsolutePath);
		return nullptr;
	}

	// Import mesh
	bool bOperationCancelled = false;
	UObject *MeshObj = FbxFactory->ImportObject(
			UStaticMesh::StaticClass(), InPackage, FName(*InMeshName), RF_Transactional | RF_Standalone | RF_Public, MeshAbsolutePath, nullptr, bOperationCancelled);

	// If import has been cancelled
	if (bOperationCancelled)
	{
		return nullptr;
	}

	FAssetRegistryModule::AssetCreated(MeshObj);

	// Mark outer dirty
	if (DataAsset->GetOuter())
	{
		DataAsset->GetOuter()->MarkPackageDirty();
	}
	else
	{
		DataAsset->MarkPackageDirty();
	}
	// Mark mesh dirty
	if (MeshObj)
	{
		MeshObj->MarkPackageDirty();
	}

	// Return the cast result
	return Cast<UStaticMesh>(MeshObj);
}

void FSDFParserEditor::CreateCollisionForMesh(UStaticMesh *&OutMesh, const ESDFGeometryType &Type)
{
	switch (Type)
	{
	case ESDFGeometryType::None:
		return;
	case ESDFGeometryType::Mesh:
		return;
	case ESDFGeometryType::Box:
		RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopX10);
		return;
	case ESDFGeometryType::Cylinder:
		RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopZ10);
		return;
	case ESDFGeometryType::Sphere:
		RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopX10);
		return;
	default:
		UE_LOG(LogTemp, Error, TEXT("GeometryType not supported for %s."), *OutMesh->GetName());
		return;
	}
}