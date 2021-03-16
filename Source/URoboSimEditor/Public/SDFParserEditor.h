// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "SDFParserInterface.h"

class FAssetRegistryModule;

class UROBOSIMEDITOR_API FSDFParserEditor : public ISDFParserInterface
{
public:
	// Default constructor
	FSDFParserEditor();

	// Constructor with load from path
	FSDFParserEditor(const FString &InFilename);

	// Destructor
	~FSDFParserEditor();

	// Clear parser
	virtual void Clear() override;

	// Check if sdf data is valid
	virtual const bool IsValidSDF();

	// Create data asset and parse sdf data into it
	virtual USDFDataAsset *ParseToDataAsset(UObject *&InParent, FName &InName, EObjectFlags &InFlags);

private:
	// Import .fbx meshes from data asset
	UStaticMesh *ImportMesh(const FString &Uri, const FString &InMeshName, UPackage *InPackage);

	// Create meshes from data asset
	void CreateMeshes();
	void CreateMesh(const FString &InMeshName, const FString &LinkName, USDFGeometry *OutGeometry);

	// Load sdf from path
	virtual const bool LoadSDF(const FString &InFilename);

	// Fix file path
	void SetDirectoryPath(const FString &InFilename);

	// Create package name
	FString CreatePackageName(const FString &InMeshName, const FString &LinkName);

	// Get mesh absolute path
	FString GetMeshAbsolutePath(const FString &Uri);

	// Create collision for custom mesh
	void CreateCollisionForMesh(UStaticMesh *&OutMesh, const ESDFGeometryType &Type);

	FString DirPath;

	UFbxFactory *FbxFactory;

	FAssetRegistryModule &AssetRegistryModule;
};
