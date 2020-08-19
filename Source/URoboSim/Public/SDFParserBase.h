
//// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
//// Author: Michael Neumann

//#pragma once

//#include "CoreMinimal.h"
//#include "UObject/NoExportTypes.h"
//#include "Engine/StaticMesh.h"
//#include "SDF/SDFDataAsset.h"
//#include "SDF/SDFGeometry.h"
//#include "AssetRegistryModule.h"
//#include "ObjectMacros.h" // EObjectFlags

//// Forward declaration
//class FXmlFile;
//class FXmlNode;
//class UFbxFactory;
//// class USDFDataAsset;
//class USDFModel;
//class USDFJoint;
//class USDFLink;
//class USDFVisual;
//class USDFCollision;
//// class USDFGeometry;

///**
//*
//*/
//enum class ESDFType : uint8
//{
//    None,
//    Visual,
//    Collision
//};


///**
//* SDF parser class
//*/

//class UROBOSIM_API FSDFParserBase
//{

//public:
//    virtual bool LoadSDF(const FString& InFilename);
//    virtual void clear();


//protected:


//    protected: //Variables
//    // Reader for the xml file
//    FXmlFile* XmlFile;

//    // Flag if parser is loaded
//    bool bSDFLoaded;

//    // Pointer to the generated data asset
//   // USDFDataAsset* DataAsset;

////    // Cached directory path
////    FString DirPath;

////    FString CurrentLinkName;

////    // Fbx factory
////    UFbxFactory* FbxFactory;

////    FAssetRegistryModule& AssetRegistryModule;
//    };


