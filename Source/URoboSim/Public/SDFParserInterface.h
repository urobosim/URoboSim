// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/StaticMesh.h"
#include "SDF/SDFDataAsset.h"
#include "SDF/SDFGeometry.h"
#include "AssetRegistryModule.h"
#include "UObject/ObjectMacros.h" // EObjectFlags
#include "SDFParserInterface.generated.h"

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
*
*/
enum class ESDFType : uint8
{
    None,
    Visual,
    Collision
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USDFParserInterface : public UInterface
{
    GENERATED_BODY()
};

/**
 *
 */
class UROBOSIM_API ISDFParserInterface
{
    GENERATED_BODY()

    // Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
//        // Load sdf from path
        virtual bool LoadSDF(const FString& InFilename);

//        // Clear parser
        virtual void Clear(); //

//        // Create data asset and parse sdf data into it
        virtual USDFDataAsset* ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags);

//    protected:
//        /* Begin parser functions */
//        // Check if sdf data is valid
        virtual bool IsValidSDF(); //

//        // Parse <sdf> node
        virtual void ParseSDF();

        // Parse <model> node
        virtual void ParseModel(const FXmlNode* InNode);

        // Parse <link> node
        virtual void ParseLink(const FXmlNode* InNode, USDFModel*& OutModel);

        // Parse <link> <inertial> node
        virtual void ParseLinkInertial(const FXmlNode* InNode, USDFLink*& OutLink); //

        // Parse <visual> node
        virtual void ParseVisual(const FXmlNode* InNode, USDFLink*& OutLink);

        // Parse <collision> node
        virtual void ParseCollision(const FXmlNode* InNode, USDFLink*& OutLink);

        // Parse <geometry> node
        virtual void ParseGeometry(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type); //

        // Parse <geometry> <mesh> node
        virtual void ParseGeometryMesh(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type);

        // Parse <geometry> <box> node
        virtual void ParseGeometryBox(const FXmlNode* InNode, USDFGeometry*& OutGeometry);//

        // Parse <geometry> <cylinder> node
        virtual void ParseGeometryCylinder(const FXmlNode* InNode, USDFGeometry*& OutGeometry);//

        // Parse <geometry> <sphere> node
        virtual void ParseGeometrySphere(const FXmlNode* InNode, USDFGeometry*& OutGeometry);//

        // Parse <joint> node
        virtual void ParseJoint(const FXmlNode* InNode, USDFModel*& OutModel);//

        // Parse <joint> <axis> node
        virtual void ParseJointAxis(const FXmlNode* InNode, USDFJoint*& OutJoint);//

        // Parse <joint> <axis> <limit> node
        virtual void ParseJointAxisLimit(const FXmlNode* InNode, USDFJoint*& OutJoint);//
        /* End parser functions */

        /*Beginner of Helper Functions*/
        // From <pose>z y z r p y</pose> to FTransform
        virtual FTransform PoseContentToFTransform(const FString& InPoseData);//

        // From <size>z y z</size> to FVector
        virtual FVector SizeToFVector(const FString& InSizeData);//

        // From <xzy>z y z</xzy> to FVector
        virtual FVector XyzToFVector(const FString& InXyzData);//
        /* End helper functions */


    protected: //Variables
        // Reader for the xml file
        FXmlFile* XmlFile;

        // Flag if parser is loaded
        bool bSDFLoaded;


        // Pointer to the generated data asset
//        USDFDataAsset* DataAsset;

//        FString CurrentLinkName;

};
