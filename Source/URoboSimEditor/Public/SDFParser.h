// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "SDF/SDFDataAsset.h"
#include "SDF/SDFGeometry.h"
#include "AssetRegistryModule.h"
#include "ObjectMacros.h" // EObjectFlags
#include "SDFParserBase.h"
//#include "SDFParserInterface.h"

// Forward declaration
class FXmlFile;
class FXmlNode;
class UFbxFactory;
// class USDFDataAsset;
class USDFModel;
class USDFJoint;
class USDFLink;
class USDFVisual;
class USDFCollision;
// class USDFGeometry;


/**
* SDF parser class
*/
class UROBOSIM_API FSDFParser : public FSDFParserBase
{
public:
	// Default constructor
	FSDFParser();

	// Constructor with load from path
	FSDFParser(const FString& InFilename);

	// Destructor
	~FSDFParser();

    virtual bool LoadSDF (const FString& InFilename) override;

	// Create data asset and parse sdf data into it
    virtual USDFDataAsset* ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags) override;

protected:
    /* Begin parser functions */
    // Parse <link> node
    virtual void ParseLink(const FXmlNode* InNode, USDFModel*& OutModel) override;

	// Parse <link> node
    virtual void ParseVisual(const FXmlNode* InNode, USDFLink*& OutLink) override; //possible there are some errors for the runtime verison

	// Parse <geometry> <mesh> node
    virtual void ParseGeometryMesh(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type) override; // Also maybe

	/* End parser functions */
    virtual void ParseCollision(const FXmlNode* InNode, USDFLink*& OutLink) override;

	/* Begin helper functions */
	// Fix file path
	void SetDirectoryPath(const FString& InFilename);

	// Get mesh absolute path
	FString GetMeshAbsolutePath(const FString& Uri);

    FName GenerateMeshName(ESDFType InType, FString InName);
    FString GeneratePackageName(FName MeshName);
    bool CreateCollisionForMesh(UStaticMesh* OutMesh, ESDFGeometryType Type);
    USDFCollision* CreateVirtualCollision(USDFLink* OutLink);

	// Import .fbx meshes from data asset
	UStaticMesh* ImportMesh(const FString& Uri, ESDFType Type);
	UStaticMesh* CreateMesh(ESDFType InType, ESDFGeometryType InShape, FString InName, TArray<float> InParameters);

	/* End helper functions */

	// Cached directory path
	FString DirPath;

	// Fbx factory
	UFbxFactory* FbxFactory;

    FAssetRegistryModule& AssetRegistryModule;
};
