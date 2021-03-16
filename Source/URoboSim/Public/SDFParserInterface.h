// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMesh.h"
#include "Factories/FbxFactory.h"
#include "SDF/SDFDataAsset.h"
#include "UObject/Interface.h"
#include "XmlFile.h"
// clang-format off
#include "SDFParserInterface.generated.h"
// clang-format on

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USDFParserInterface : public UInterface
{
	GENERATED_BODY()
};

class UROBOSIM_API ISDFParserInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Clear parser
	virtual void Clear();

	// /* Begin parser functions */
	// Parse <sdf> node
	virtual void ParseSDF();

	// Parse <model> node
	virtual void ParseModel(const FXmlNode *InNode);

	// Parse <link> node
	virtual void ParseLink(const FXmlNode *InNode, USDFModel *&OutModel);

	// Parse <link> <inertial> node
	virtual void ParseLinkInertial(const FXmlNode *InNode, USDFLink *&OutLink);

	// Parse <link> <inertial> <inertia> node
	virtual void ParseLinkInertialInertia(const FXmlNode *InNode, USDFLink *&OutLink);

	// Parse <visual> node
	virtual void ParseVisual(const FXmlNode *InNode, USDFLink *&OutLink);

	// Parse <collision> node
	virtual void ParseCollision(const FXmlNode *InNode, USDFLink *&OutLink);

	// Parse <visual> <geometry> node
	virtual void ParseVisualGeometry(const FXmlNode *InNode, USDFVisual *&OutVisual);

	// Parse <collision> <geometry> node
	virtual void ParseCollisionGeometry(const FXmlNode *InNode, USDFCollision *&OutCollision);

	// Parse <geometry> node
	virtual void ParseGeometry(const FXmlNode *InNode, USDFGeometry *&OutGeometry);

	// Parse <geometry> <mesh> node
	virtual void ParseGeometryMesh(const FXmlNode *InNode, USDFGeometry *&OutGeometry);

	// Parse <geometry> <box> node
	virtual void ParseGeometryBox(const FXmlNode *InNode, USDFGeometry *&OutGeometry);

	// Parse <geometry> <cylinder> node
	virtual void ParseGeometryCylinder(const FXmlNode *InNode, USDFGeometry *&OutGeometry);

	// Parse <geometry> <sphere> node
	virtual void ParseGeometrySphere(const FXmlNode *InNode, USDFGeometry *&OutGeometry);

	// Parse <joint> node
	virtual void ParseJoint(const FXmlNode *InNode, USDFModel *&OutModel);

	// Parse <joint> <axis> node
	virtual void ParseJointAxis(const FXmlNode *InNode, USDFJoint *&OutJoint);

	// Parse <joint> <axis> <limit> node
	virtual void ParseJointAxisLimit(const FXmlNode *InNode, USDFJoint *&OutJoint);
	// /* End parser functions */

public:
	/*Beginner of Helper Functions*/
	// From <pose>z y z r p y</pose> to FTransform
	virtual const FTransform PoseContentToFTransform(const FString &InPoseData);

	// From <scale>x y z</scale> to FVector
	virtual const FVector ScaleContentToFVector(const FString &InScaleData);

	// From <size>z y z</size> to FVector
	virtual const FVector SizeToFVector(const FString &InSizeData);

	// From <xzy>z y z</xzy> to FVector
	virtual const FVector XyzToFVector(const FString &InXyzData);
	/* End helper functions */

protected:
	// Reader for the xml file
	FXmlFile *XmlFile;

	// Flag if parser is loaded
	bool bSDFLoaded;

	// Pointer to the generated data asset
	USDFDataAsset *DataAsset;
};
