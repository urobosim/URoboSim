// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "SDF/SDFDataAsset.h"
#include "SDF/SDFGeometry.h"
#if WITH_EDITOR
#include "AssetRegistryModule.h"
#endif
#include "UObject/ObjectMacros.h" // EObjectFlags
//#include "SDFParserBase.h"
#include "SDFParserInterface.h"

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
class UROBOSIMEDITOR_API FSDFParser :public ISDFParserInterface //: public FSDFParserBase
{
public:
    // Default constructor
    FSDFParser();

    // Constructor with load from path
    FSDFParser(const FString& InFilename);

    // Destructor
    ~FSDFParser();

    // Load sdf from path
    virtual bool LoadSDF(const FString& InFilename) override;

    // Clear parser
    virtual void Clear() override;

    // Create data asset and parse sdf data into it
    virtual USDFDataAsset* ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags)override;

private:
    /* Begin parser functions */
    // Check if sdf data is valid
    virtual bool IsValidSDF() override;

    // Parse <sdf> node
    virtual void ParseSDF() override;

    //Get ROS package path
    virtual FString GetROSPackagePath(const FString& InPackageName);
    virtual void GetROSPackagePaths();

    // Parse <model> node
    virtual void ParseModel(const FXmlNode* InNode) override;

    // Parse <link> node
    virtual void ParseLink(const FXmlNode* InNode, USDFModel*& OutModel) override;

    // Parse <joint> node
    virtual void ParseJoint(const FXmlNode* InNode, USDFModel*& OutModel) override;//

    virtual void ParsePlugin(const FXmlNode* InNode, USDFModel*& OutModel) ;
    // Parse <link> <inertial> node
    
    // Parse <visual> node
    virtual void ParseVisual(const FXmlNode* InNode, USDFLink*& OutLink) override;

    // Parse <collision> node
    virtual void ParseCollision(const FXmlNode* InNode, USDFLink*& OutLink) override;

    // Parse <geometry> node

    // Parse <geometry> <mesh> node
    virtual void ParseGeometryMesh(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type) override;

    // Fix file path
    void SetDirectoryPath(const FString& InFilename);

    // Get mesh absolute path
    FString GetMeshAbsolutePath(const FString& Uri);

    FName GenerateMeshName(ESDFType InType, FString InName);
    FString GeneratePackageName(FName MeshName, FString InPackagePath);

    TMap<FString,FString> ROSPackagePaths;

////    // Pointer to the generated data asset
    USDFDataAsset* DataAsset;

    // Cached directory path
    FString DirPath;

    FString CurrentLinkName;

#if WITH_EDITOR
    // Fbx factory
    UFbxFactory* FbxFactory;
    FAssetRegistryModule& AssetRegistryModule;
    
    // Import .fbx meshes from data asset
    UStaticMesh* ImportMesh(const FString& Uri, ESDFType Type);
    UStaticMesh* CreateMesh(ESDFType InType, ESDFGeometryType InShape, FString InName, TArray<float> InParameters);

    bool CreateCollisionForMesh(UStaticMesh* OutMesh, ESDFGeometryType Type);
    USDFCollision* CreateVirtualCollision(USDFLink* OutLink);
#endif
};
