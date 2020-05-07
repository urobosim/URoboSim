// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "SDF/SDFParser.h"
#include "SDF/SDFDataAsset.h"
#include "Conversions.h"
#include "Paths.h"
#include "XmlFile.h"
#include "Factories/FbxFactory.h"
#include "AssetRegistryModule.h"
#include "RStaticMeshEditUtils.h"

// Default constructor
FSDFParser::FSDFParser() : XmlFile(nullptr), bSDFLoaded(false)
{
}

// Constructor with load from path
FSDFParser::FSDFParser(const FString& InFilename) : XmlFile(nullptr), bSDFLoaded(false)
{
  LoadSDF(InFilename);
}

// Destructor
FSDFParser::~FSDFParser()
{
  Clear();
}

// Load sdf from file
bool FSDFParser::LoadSDF(const FString& InFilename)
{
  AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
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

// Clear parser
void FSDFParser::Clear()
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
      DataAsset = nullptr;
      FbxFactory = nullptr;
      DirPath = TEXT("");
    }
}

// Create data asset and parse sdf data into it
USDFDataAsset* FSDFParser::ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags)
{
  if (!bSDFLoaded)
    {
      return nullptr;
    }

  // Create a new SDFDataAsset
  DataAsset = NewObject<USDFDataAsset>(InParent, InName, InFlags);

  // Parse sdf data and fill the data asset
  ParseSDF();

  return DataAsset;
}

// Check if sdf data is valid
bool FSDFParser::IsValidSDF()
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

// Parse <sdf> node
void FSDFParser::ParseSDF()
{
  // Get "version" from node attribute
  const FString SDFVersion = XmlFile->GetRootNode()->GetAttribute(TEXT("version"));
  if (!SDFVersion.IsEmpty())
    {
      DataAsset->Version = SDFVersion;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <sdf> has no \"version\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      DataAsset->Version = TEXT("__default__");
    }

  // Iterate <sdf> child nodes
  for (const auto& ChildNode : XmlFile->GetRootNode()->GetChildrenNodes())
    {
      // Check if <model>
      if (ChildNode->GetTag().Equals(TEXT("model")))
        {
          ParseModel(ChildNode);
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <sdf> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

// Parse <model> node
void FSDFParser::ParseModel(const FXmlNode* InNode)
{
  // Ptr to the new model
  USDFModel* NewModel = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if(!Name.IsEmpty())
    {
      NewModel = NewObject<USDFModel>(DataAsset, FName(*Name));
      NewModel->Name = Name;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <model> has no \"name\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      NewModel = NewObject<USDFModel>(DataAsset/*, FName(TEXT("__default__"))*/);
      NewModel->Name = TEXT("__default__");
    }

  // Iterate <model> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("link")))
        {
          ParseLink(ChildNode, NewModel);
        }
      else if (ChildNode->GetTag().Equals(TEXT("joint")))
        {
          ParseJoint(ChildNode, NewModel);
        }
      else if (ChildNode->GetTag().Equals(TEXT("static")))
        {
          NewModel->bStatic = ChildNode->GetContent().ToBool();
        }
      else if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          NewModel->Pose = PoseContentToFTransform(ChildNode->GetContent());
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <model> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }

  // Add model to the data asset
  DataAsset->Models.Add(NewModel);
}

// Parse <link> node
void FSDFParser::ParseLink(const FXmlNode* InNode, USDFModel*& OutModel)
{
  // Ptr to the new link
  USDFLink* NewLink = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty())
    {
      NewLink = NewObject<USDFLink>(OutModel, FName(*Name));
      NewLink->Name = Name;
      CurrentLinkName = Name;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> has no \"name\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      NewLink = NewObject<USDFLink>(OutModel/*, FName(TEXT("__default__"))*/);
      NewLink->Name = TEXT("__default__");
    }

  // Iterate <link> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          NewLink->Pose = PoseContentToFTransform(ChildNode->GetContent());
        }
      else if (ChildNode->GetTag().Equals(TEXT("inertial")))
        {
          ParseLinkInertial(ChildNode, NewLink);
        }
      else if (ChildNode->GetTag().Equals(TEXT("visual")))
        {
          ParseVisual(ChildNode, NewLink);
        }
      else if (ChildNode->GetTag().Equals(TEXT("collision")))
        {
          ParseCollision(ChildNode, NewLink);
        }
      else if (ChildNode->GetTag().Equals(TEXT("self_collide")))
        {
          NewLink->bSelfCollide = ChildNode->GetContent().ToBool();
        }
      else if (ChildNode->GetTag().Equals(TEXT("gravity")))
        {
          NewLink->bGravity = ChildNode->GetContent().ToBool();
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
  if(NewLink->Collisions.Num() == 0)
    {
      USDFCollision* Collision = CreateVirtualCollision(NewLink);
      if(Collision)
        {
          NewLink->Collisions.Add(Collision);
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Creation of Virtual Link % failed"), *CurrentLinkName);
        }
    }

  // Add link to the data asset
  OutModel->Links.Add(NewLink);
}

// Parse <link> <inertial> node
void FSDFParser::ParseLinkInertial(const FXmlNode* InNode, USDFLink*& OutLink)
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

// Parse <visual> node
void FSDFParser::ParseVisual(const FXmlNode* InNode, USDFLink*& OutLink)
{
  // Ptr to the new visual
  USDFVisual* NewVisual = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty())
    {
      NewVisual = NewObject<USDFVisual>(OutLink, FName(*Name));
      NewVisual->Name = Name;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> has no \"name\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      NewVisual = NewObject<USDFVisual>(OutLink/*, FName(TEXT("__default__"))*/);
      NewVisual->Name = TEXT("__default__");
    }

  // Iterate <visual> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          NewVisual->Pose = PoseContentToFTransform(ChildNode->GetContent());
        }
      else if (ChildNode->GetTag().Equals(TEXT("geometry")))
        {
          ParseGeometry(ChildNode, NewVisual->Geometry, ESDFType::Visual);
          if(NewVisual->Geometry->Type == ESDFGeometryType::Box ||
             NewVisual->Geometry->Type == ESDFGeometryType::Cylinder ||
             NewVisual->Geometry->Type == ESDFGeometryType::Sphere)
            {
              NewVisual->Geometry->Mesh = CreateMesh(ESDFType::Visual, NewVisual->Geometry->Type, Name, RStaticMeshUtils::GetGeometryParameter(NewVisual->Geometry));
            }
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> <visual> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }

  // Add visual to array
  OutLink->Visuals.Add(NewVisual);
}

// Parse <collision> node
void FSDFParser::ParseCollision(const FXmlNode* InNode, USDFLink*& OutLink)
{
  // Ptr to the new collision
  USDFCollision* NewCollision = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty())
    {
      NewCollision = NewObject<USDFCollision>(OutLink, FName(*Name));
      NewCollision->Name = Name;
    }
  else
    {
      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <collision> has no \"name\" attribute, added a default value.."),
             *FString(__FUNCTION__), __LINE__);
      NewCollision = NewObject<USDFCollision>(OutLink/*, FName(TEXT("__default__"))*/);
      NewCollision->Name = TEXT("__default__");
    }

  // Iterate <collision> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          NewCollision->Pose = PoseContentToFTransform(ChildNode->GetContent());
        }
      else if (ChildNode->GetTag().Equals(TEXT("geometry")))
        {
          ParseGeometry(ChildNode, NewCollision->Geometry, ESDFType::Collision);
          if(NewCollision->Geometry->Type == ESDFGeometryType::Box ||
             NewCollision->Geometry->Type == ESDFGeometryType::Cylinder ||
             NewCollision->Geometry->Type == ESDFGeometryType::Sphere)
            {
              NewCollision->Geometry->Mesh = CreateMesh(ESDFType::Collision, NewCollision->Geometry->Type, Name, RStaticMeshUtils::GetGeometryParameter(NewCollision->Geometry));
              // RStaticMeshUtils::CreateStaticMeshThroughBrush(OutLink,NewCollision);
            }
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <inertial> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }

  // Add collision to array
  OutLink->Collisions.Add(NewCollision);
}

// Parse <geometry> node
void FSDFParser::ParseGeometry(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type)
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

// Parse <geometry> <mesh> node
void FSDFParser::ParseGeometryMesh(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type)
{
  // Set geometry type
  OutGeometry->Type = ESDFGeometryType::Mesh;

  // Iterate <geometry> <mesh> child nodes
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("uri")))
        {
          // Import mesh, set Uri as the relative path from the asset to the mesh uasset
          OutGeometry->Uri = ChildNode->GetContent();
          OutGeometry->Mesh = ImportMesh(OutGeometry->Uri, Type);
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <mesh> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}

// Parse <geometry> <box> node
void FSDFParser::ParseGeometryBox(const FXmlNode* InNode, USDFGeometry*& OutGeometry)
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
void FSDFParser::ParseGeometryCylinder(const FXmlNode* InNode, USDFGeometry*& OutGeometry)
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
void FSDFParser::ParseGeometrySphere(const FXmlNode* InNode, USDFGeometry*& OutGeometry)
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
void FSDFParser::ParseJoint(const FXmlNode* InNode, USDFModel*& OutModel)
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
void FSDFParser::ParseJointAxis(const FXmlNode* InNode, USDFJoint*& OutJoint)
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
void FSDFParser::ParseJointAxisLimit(const FXmlNode* InNode, USDFJoint*& OutJoint)
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


/* Begin helper functions */
// Fix file path
void FSDFParser::SetDirectoryPath(const FString& InFilename)
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

// Get mesh absolute path
FString FSDFParser::GetMeshAbsolutePath(const FString& Uri)
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

FName FSDFParser::GenerateMeshName(ESDFType InType, FString InName)
{
  FName MeshName;
  if (InType == ESDFType::Collision)
    {
      MeshName = FName(*(TEXT("SM_") + InName + TEXT("_C")));
    }
  else if (InType == ESDFType::Visual)
    {
      MeshName = FName(*(TEXT("SM_") + InName + TEXT("_V")));
    }
  return MeshName;
}

FString FSDFParser::GeneratePackageName(FName MeshName)
{
  FString PackageName = "";
  FString Reason = "";
  FString NewDir = DataAsset->GetOuter()->GetPathName() + "/" + CurrentLinkName;
  if(!FPackageName::TryConvertFilenameToLongPackageName(NewDir + "/" + MeshName.ToString() , PackageName, &Reason))
    {
      UE_LOG(LogTemp, Error, TEXT("Packacke name invlaide because : %s"), *Reason);
    }
  AssetRegistryModule.Get().AddPath(NewDir);
  return PackageName;
}

UStaticMesh* FSDFParser::CreateMesh(ESDFType InType, ESDFGeometryType InShape, FString InName, TArray<float> InParameters)
{
  // FString Path = "";
  FName MeshName = GenerateMeshName(InType, InName);
  FString PackageName = GeneratePackageName(MeshName);


  UPackage* Pkg = CreatePackage(NULL, *PackageName);
  UStaticMesh* Mesh = RStaticMeshUtils::CreateStaticMesh(Pkg, PackageName, InShape, InParameters);
  CreateCollisionForMesh(Mesh, InShape);
  return Mesh;
}

// Import .fbx meshes from data asset
UStaticMesh* FSDFParser::ImportMesh(const FString& Uri, ESDFType Type)
{
  FString MeshAbsolutePath = GetMeshAbsolutePath(Uri);
  if (!FPaths::FileExists(MeshAbsolutePath))
    {
      UE_LOG(LogTemp, Error, TEXT("[%s] Could not find %s"), *FString(__FUNCTION__), *MeshAbsolutePath);
      return nullptr;
    }

  // Get mesh name

  FString MeshNameTemp = FPaths::GetBaseFilename(MeshAbsolutePath);
  FName MeshName = GenerateMeshName(Type, MeshNameTemp);

  // Import mesh
  bool bOperationCancelled = false;

  FString PackageName = GeneratePackageName(MeshName);
  UPackage* Pkg = CreatePackage(NULL, *PackageName);
  UObject* MeshObj = FbxFactory->ImportObject(
                                              UStaticMesh::StaticClass(), Pkg, MeshName, RF_Transactional | RF_Standalone | RF_Public, MeshAbsolutePath, nullptr, bOperationCancelled);

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

// From <pose>z y z r p y</pose> to FTransform
FTransform FSDFParser::PoseContentToFTransform(const FString& InPoseData)
{
  // <pose>x=[0] y=[1] z=[2] r=[3] p=[4] y=[5]</pose>
  TArray<FString> PoseDataArray;
  int32 ArrSize = InPoseData.ParseIntoArray(PoseDataArray, TEXT(" "), true);

  if (ArrSize == 6)
    {
      // roll [3], pitch [4], yaw [5] --> pitch [4], yaw [5], roll [3]
      const FRotator Rot(FMath::RadiansToDegrees(FCString::Atof(*PoseDataArray[4]) < 0 ? FCString::Atof(*PoseDataArray[4]) + 2*PI : FCString::Atof(*PoseDataArray[4])),	// pitch
                         FMath::RadiansToDegrees(FCString::Atof(*PoseDataArray[5])< 0 ? FCString::Atof(*PoseDataArray[5]) + 2*PI : FCString::Atof(*PoseDataArray[5])),		// yaw
                         FMath::RadiansToDegrees(FCString::Atof(*PoseDataArray[3])< 0 ? FCString::Atof(*PoseDataArray[3]) + 2*PI : FCString::Atof(*PoseDataArray[3])));		// roll

      const FVector Loc = FVector(FCString::Atof(*PoseDataArray[0]),
                                  FCString::Atof(*PoseDataArray[1]),
                                  FCString::Atof(*PoseDataArray[2]));

      const FTransform Trans = FConversions::ROSToU(FTransform(Rot, Loc));

      return Trans;
    }

  UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <pose>%s</pose> is an unsupported format!"),
         *FString(__FUNCTION__), __LINE__, *InPoseData);
  return FTransform();
}

// From <>z y z</> to FVector
FVector FSDFParser::XyzToFVector(const FString& InXyzData)
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
FVector FSDFParser::SizeToFVector(const FString& InSizeData)
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

bool FSDFParser::CreateCollisionForMesh(UStaticMesh* OutMesh, ESDFGeometryType Type)
{
  switch(Type)
    {
    case ESDFGeometryType::None :
      return false;
    case ESDFGeometryType::Mesh :
      return true;
    case ESDFGeometryType::Box :
      RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopX10);
      return true;
    case ESDFGeometryType::Cylinder :
      RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopZ10);
      return true;
    case ESDFGeometryType::Sphere :
      RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopX10);
      return true;
    default :
      UE_LOG(LogTemp, Error, TEXT("GeometryType not supportet for %s."), *OutMesh->GetName());
      return false;
    }
}

USDFCollision* FSDFParser::CreateVirtualCollision(USDFLink* OutLink)
{
  USDFCollision* NewCollision = NewObject<USDFCollision>(OutLink, FName(*CurrentLinkName));
  NewCollision->Name = CurrentLinkName;
  NewCollision->Pose = FTransform();
  NewCollision->Geometry = NewObject<USDFGeometry>(NewCollision);
  NewCollision->Geometry->Type = ESDFGeometryType::Box;
  NewCollision->Geometry->Size = FVector(0.5f, 0.5f, 0.5f);
  NewCollision->Geometry->Mesh = CreateMesh(ESDFType::Collision, ESDFGeometryType::Box, CurrentLinkName, RStaticMeshUtils::GetGeometryParameter(NewCollision->Geometry));
  return NewCollision;
}
