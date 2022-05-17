// Fill out your copyright notice in the Description page of Project Settings.


#include "SDFParserInterface.h"
// Fill out your copyright notice in the Description page of Project Settings.
#include "XmlFile.h"
#include "Conversions.h"


// Load sdf from path
bool ISDFParserInterface::LoadSDF(const FString& InFilename)
{
    // todo rm make pure
    return false;
}

USDFDataAsset* ISDFParserInterface::ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags)
{
    // todo rm make pure
    return nullptr;
}

void ISDFParserInterface::ParseSDF()
{
    // todo rm make pure
}

// Parse <model> node
void ISDFParserInterface::ParseModel(const FXmlNode* InNode)
{
    // todo rm make pure
}

// Parse <link> node
void ISDFParserInterface::ParseLink(const FXmlNode* InNode, USDFModel*& OutModel)
{
    // todo rm make pure
}

void ISDFParserInterface::ParseVisual(const FXmlNode* InNode, USDFLink*& OutLink)
{
    // todo rm make pure
}

// Parse <collision> node
void ISDFParserInterface::ParseCollision(const FXmlNode* InNode, USDFLink*& OutLink)
{
    // todo rm make pure
}

// Parse <geometry> <mesh> node
void ISDFParserInterface::ParseGeometryMesh(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type)
{
    // todo rm make pure
}

// Clear parser
void ISDFParserInterface::Clear()
{
  if (XmlFile)
    {
      XmlFile->Clear();
      delete XmlFile;
      XmlFile = nullptr;
    }
  if (bSDFLoaded)
    {
      bSDFLoaded = false;
//      DataAsset = nullptr;
    }
}

//// Create data asset and parse sdf data into it
//USDFDataAsset* ISDFParserInterface::ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags)
//{
//  if (!bSDFLoaded)
//    {
//      return nullptr;
//    }

//  // Create a new SDFDataAsset
//  DataAsset = NewObject<USDFDataAsset>(InParent, InName, InFlags);

//  // Parse sdf data and fill the data asset
//  this->ParseSDF();

//  return DataAsset;
//}

// Check if sdf data is valid
bool ISDFParserInterface::IsValidSDF()
{
  if (XmlFile == nullptr)
    {
      return false;
    }

  // Check if root node is <sdf> or <gazebo> (sdf version 1.2)
  if (!XmlFile->GetRootNode()->GetTag().Equals(TEXT("sdf"))
      && !XmlFile->GetRootNode()->GetTag().Equals(TEXT("gazebo")))
    {
      // UE_LOG(LogTemp, Error, TEXT("[%s][%d] Root node is not <sdf> or <gazebo>(sdf version 1.2)"), TEXT(__FUNCTION__), __LINE__);
      return false;
    }
  return true;
}


// Parse <link> <inertial> node
void ISDFParserInterface::ParseLinkInertial(const FXmlNode* InNode, USDFLink*& OutLink)
{
  // Iterate <link> <inertial> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("mass")))
        {
          OutLink->Inertial->Mass = FCString::Atof(*ChildNode->GetContent());
        }
      else if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          OutLink->Inertial->Pose = PoseContentToFTransform(ChildNode->GetContent());
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <inertial> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

// Parse <geometry> node
void ISDFParserInterface::ParseGeometry(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type)
{
  // Iterate <geometry> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("mesh")))
        {
          ParseGeometryMesh(ChildNode, OutGeometry, Type);
        }
      else if (ChildNode->GetTag().Equals(TEXT("box")))
        {
          ParseGeometryBox(ChildNode, OutGeometry);
        }
      else if (ChildNode->GetTag().Equals(TEXT("cylinder")))
        {
          ParseGeometryCylinder(ChildNode, OutGeometry);
        }
      else if (ChildNode->GetTag().Equals(TEXT("sphere")))
        {
          ParseGeometrySphere(ChildNode, OutGeometry);
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

//// Parse <geometry> <mesh> node
//void ISDFParserInterface::ParseGeometryMesh(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type)
//{
//  // Set geometry type
//  OutGeometry->Type = ESDFGeometryType::Mesh;

//  // Iterate <geometry> <mesh> child nodes
//  for (const auto& ChildNode : InNode->GetChildrenNodes())
//    {
//      if (ChildNode->GetTag().Equals(TEXT("uri")))
//        {
//          // Import mesh, set Uri as the relative path from the asset to the mesh uasset
//          OutGeometry->Uri = ChildNode->GetContent();
//          OutGeometry->Mesh = ImportMesh(OutGeometry->Uri, Type);
//        }
//      else
//        {
//          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <mesh> child <%s> not supported, ignored.."),
//                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
//          continue;
//        }
//    }
//}

// Parse <geometry> <box> node
void ISDFParserInterface::ParseGeometryBox(const FXmlNode* InNode, USDFGeometry*& OutGeometry)
{
  // Set geometry type
  OutGeometry->Type = ESDFGeometryType::Box;

  // Iterate <geometry> <box> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("size")))
        {
          OutGeometry->Size = SizeToFVector(ChildNode->GetContent());
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <box> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }

}

// Parse <geometry> <cylinder> node
void ISDFParserInterface::ParseGeometryCylinder(const FXmlNode* InNode, USDFGeometry*& OutGeometry)
{
  // Set geometry type
  OutGeometry->Type = ESDFGeometryType::Cylinder;

  // Iterate <geometry> <cylinder> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("radius")))
        {
          OutGeometry->Radius = FConversions::MToCm(
                                                    FCString::Atof(*ChildNode->GetContent()));
        }
      else if (ChildNode->GetTag().Equals(TEXT("length")))
        {
          OutGeometry->Length = FConversions::MToCm(
                                                    FCString::Atof(*ChildNode->GetContent()));
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <cylinder> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }

  // if(OutGeometry->Radius != 0 && OutGeometry->Length != 0)
  // {
  //   OutGeometry->Mesh = ;
  // }
}

// Parse <geometry> <sphere> node
void ISDFParserInterface::ParseGeometrySphere(const FXmlNode* InNode, USDFGeometry*& OutGeometry)
{
  // Set geometry type
  OutGeometry->Type = ESDFGeometryType::Sphere;

  // Iterate <geometry> <sphere> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("radius")))
        {
          OutGeometry->Radius = FConversions::MToCm(
                                                    FCString::Atof(*ChildNode->GetContent()));
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <sphere> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

// Parse <joint> node
void ISDFParserInterface::ParseJoint(const FXmlNode* InNode, USDFModel*& OutModel)
{
  // Pointer to the new joint
  USDFJoint* NewJoint = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty())
    {
      NewJoint = NewObject<USDFJoint>(OutModel, FName(*Name));
      NewJoint->Name = Name;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> has no \"name\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      NewJoint = NewObject<USDFJoint>(OutModel/*, FName(TEXT("__default__"))*/);
      NewJoint->Name = TEXT("__default__");
    }

  // Get "type" from node attribute
  const FString Type = InNode->GetAttribute(TEXT("type"));
  if (!Name.IsEmpty())
    {
      NewJoint->Type = Type;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> has no \"type\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      NewJoint->Name = TEXT("__default__");
    }

  // Iterate <joint> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("parent")))
        {
          NewJoint->Parent = ChildNode->GetContent();
        }
      else if (ChildNode->GetTag().Equals(TEXT("child")))
        {
          NewJoint->Child = ChildNode->GetContent();
        }
      else if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          NewJoint->Pose = PoseContentToFTransform(ChildNode->GetContent());
        }
      else if (ChildNode->GetTag().Equals(TEXT("axis")))
        {
          ParseJointAxis(ChildNode, NewJoint);
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }

  // Add link to the data asset
  OutModel->Joints.Add(NewJoint);
}

// Parse <joint> <axis> node
void ISDFParserInterface::ParseJointAxis(const FXmlNode* InNode, USDFJoint*& OutJoint)
{
  // Iterate <joint> <axis> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("xyz")))
        {
          OutJoint->Axis->Xyz = XyzToFVector(ChildNode->GetContent());
        }
      else if (ChildNode->GetTag().Equals(TEXT("use_parent_model_frame")))
        {
          OutJoint->Axis->bUseParentModelFrame = ChildNode->GetContent().ToBool();
        }
      else if (ChildNode->GetTag().Equals(TEXT("limit")))
        {
          ParseJointAxisLimit(ChildNode, OutJoint);
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> <axis> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

// Parse <joint> <axis> <limit> node
void ISDFParserInterface::ParseJointAxisLimit(const FXmlNode* InNode, USDFJoint*& OutJoint)
{
  // Iterate <joint> <axis> <limit> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("lower")))
        {
          OutJoint->Axis->SetLowerLimitFromSDF(FCString::Atof(*ChildNode->GetContent()));

        }
      else if (ChildNode->GetTag().Equals(TEXT("upper")))
        {
          OutJoint->Axis->SetUpperLimitFromSDF(FCString::Atof(*ChildNode->GetContent()));
        }
      else if (ChildNode->GetTag().Equals(TEXT("effort")))
        {
          OutJoint->Axis->Effort = FCString::Atof(*ChildNode->GetContent());
        }
      else if (ChildNode->GetTag().Equals(TEXT("velocity")))
        {
          OutJoint->Axis->Velocity = FCString::Atof(*ChildNode->GetContent());
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> <axis> <limit> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}




// From <pose>z y z r p y</pose> to FTransform
FTransform ISDFParserInterface::PoseContentToFTransform(const FString& InPoseData)
{
  // <pose>x=[0] y=[1] z=[2] r=[3] p=[4] y=[5]</pose>
  TArray<FString> PoseDataArray;
  int32 ArrSize = InPoseData.ParseIntoArray(PoseDataArray, TEXT(" "), true);

  if (ArrSize == 6)
    {
      const FRotator Rot(FConversions::ROSToU(FCString::Atof(*PoseDataArray[3]) , FCString::Atof(*PoseDataArray[4]), FCString::Atof(*PoseDataArray[5])));		// roll

      const FVector Loc = FVector(FCString::Atof(*PoseDataArray[0]),
                                  FCString::Atof(*PoseDataArray[1]),
                                  FCString::Atof(*PoseDataArray[2]));

      const FTransform Trans = FTransform(Rot, FConversions::ROSToU(Loc));

      return Trans;
    }

  UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <pose>%s</pose> is an unsupported format!"),
         *FString(__FUNCTION__), __LINE__, *InPoseData);
  return FTransform();
}

// From <>z y z</> to FVector
FVector ISDFParserInterface::XyzToFVector(const FString& InXyzData)
{
  // <size>x=[0] y=[1] z=[2]</size>
  TArray<FString> DataArray;
  int32 ArrSize = InXyzData.ParseIntoArray(DataArray, TEXT(" "), true);

  if (ArrSize == 3)
    {
      return FVector(FCString::Atof(*DataArray[0]),
                     -FCString::Atof(*DataArray[1]),
                     FCString::Atof(*DataArray[2]));
    }

  // Unsupported <pose> format, return default transform
  UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <xyz>%s</xyz> is an unsupported format!"),
         __FUNCTION__, __LINE__, *InXyzData);
  return FVector();
}

// From <size>z y z</size> to FVector
FVector ISDFParserInterface::SizeToFVector(const FString& InSizeData)
{
  // <size>x=[0] y=[1] z=[2]</size>
  TArray<FString> DataArray;
  int32 ArrSize = InSizeData.ParseIntoArray(DataArray, TEXT(" "), true);

  if (ArrSize == 3)
    {
      return FConversions::MToCm(FVector(FCString::Atof(*DataArray[0]),
                                         FCString::Atof(*DataArray[1]),
                                         FCString::Atof(*DataArray[2])));
    }

  // Unsupported <pose> format, return default transform
  UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <size>%s</size> is an unsupported format!"),
         __FUNCTION__, __LINE__, *InSizeData);
  return FVector();
}


// Add default functionality here for any ISDFParserInterface functions that are not pure virtual.
