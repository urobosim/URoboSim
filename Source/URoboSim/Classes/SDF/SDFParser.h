// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "ObjectMacros.h" // EObjectFlags

// Forward declaration
class FXmlFile;
class FXmlNode;
class UFbxFactory;
class USDFDataAsset;
class USDFModel;
class USDFJoint;
class USDFLink;
class USDFVisual;
class USDFCollision;
class USDFGeometry;

/**
*
*/
enum class ESDFType : uint8
{
	None,
	Visual,
	Collision
};


/**
* SDF parser class
*/
class UROBOSIM_API FSDFParser
{
public:
	// Default constructor
	FSDFParser();

	// Constructor with load from path
	FSDFParser(const FString& InFilename);

	// Destructor
	~FSDFParser();

	// Load sdf from path
	bool LoadSDF(const FString& InFilename);

	// Clear parser
	void Clear();

	// Create data asset and parse sdf data into it
	USDFDataAsset* ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags);

private:
	/* Begin parser functions */
	// Check if sdf data is valid
	bool IsValidSDF();

	// Parse <sdf> node
	void ParseSDF();

	// Parse <model> node
	void ParseModel(const FXmlNode* InNode);

	// Parse <link> node
	void ParseLink(const FXmlNode* InNode, USDFModel*& OutModel);

	// Parse <link> <inertial> node
	void ParseLinkInertial(const FXmlNode* InNode, USDFLink*& OutLink);

	// Parse <visual> node
	void ParseVisual(const FXmlNode* InNode, USDFLink*& OutLink);

	// Parse <collision> node
	void ParseCollision(const FXmlNode* InNode, USDFLink*& OutLink);

	// Parse <geometry> node
	void ParseGeometry(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type);

	// Parse <geometry> <mesh> node
	void ParseGeometryMesh(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type);

	// Parse <geometry> <box> node
	void ParseGeometryBox(const FXmlNode* InNode, USDFGeometry*& OutGeometry);

	// Parse <geometry> <cylinder> node
	void ParseGeometryCylinder(const FXmlNode* InNode, USDFGeometry*& OutGeometry);

	// Parse <geometry> <sphere> node
	void ParseGeometrySphere(const FXmlNode* InNode, USDFGeometry*& OutGeometry);

	// Parse <joint> node
	void ParseJoint(const FXmlNode* InNode, USDFModel*& OutModel);

	// Parse <joint> <axis> node
	void ParseJointAxis(const FXmlNode* InNode, USDFJoint*& OutJoint);

	// Parse <joint> <axis> <limit> node
	void ParseJointAxisLimit(const FXmlNode* InNode, USDFJoint*& OutJoint);
	/* End parser functions */


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

	// From <pose>z y z r p y</pose> to FTransform
	FTransform PoseContentToFTransform(const FString& InPoseData);

	// From <size>z y z</size> to FVector
	FVector SizeToFVector(const FString& InSizeData);

	// From <xzy>z y z</xzy> to FVector
	FVector XyzToFVector(const FString& InXyzData);
	/* End helper functions */


	// Reader for the xml file
	FXmlFile* XmlFile;

	// Flag if parser is loaded
	bool bSDFLoaded;

	// Pointer to the generated data asset
	USDFDataAsset* DataAsset;

	// Cached directory path
	FString DirPath;

	FString CurrentLinkName;

	// Fbx factory
	UFbxFactory* FbxFactory;

    FAssetRegistryModule& AssetRegistryModule;
};
