// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "SDFParser.h"
#include "Conversions.h"
// #include "Paths.h"
#include "XmlFile.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "Factories/FbxFactory.h"
#include "RStaticMeshEditUtils.h"

// Default constructor
FSDFParser::FSDFParser() :  AssetRegistryModule(FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry")))
{
    this->XmlFile=nullptr;
    this->bSDFLoaded=false;
    GetROSPackagePaths();
}

// Constructor with load from path
FSDFParser::FSDFParser(const FString& InFilename) : AssetRegistryModule(FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry")))
{
  this->XmlFile=nullptr;
  this->bSDFLoaded=false;
  GetROSPackagePaths();
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
  AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
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
      else if (ChildNode->GetTag().Equals(TEXT("plugin")))
        {
          ParsePlugin(ChildNode, NewModel);
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

void FSDFParser::ParsePlugin(const FXmlNode* InNode, USDFModel*& OutModel)
{
  USDFPlugin* NewPlugin = nullptr;
  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  NewPlugin = NewObject<USDFPlugin>(OutModel, FName(*Name));
  NewPlugin->Name = Name;
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {

      if (ChildNode->GetTag().Equals(TEXT("joint")))
        {
          NewPlugin->Joint = ChildNode->GetContent();
        }
      else if (ChildNode->GetTag().Equals(TEXT("mimicJoint")))
        {
          NewPlugin->MimicJoint = ChildNode->GetContent();
        }
      else if (ChildNode->GetTag().Equals(TEXT("multiplier")))
        {
          NewPlugin->Multiplier = FCString::Atof(*ChildNode->GetContent());
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
      OutModel->Plugins.Add(NewPlugin);
    }

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
          UE_LOG(LogTemp, Error, TEXT("Creation of Virtual Link %s failed"), *CurrentLinkName);
        }
    }

  // Add link to the data asset
  OutModel->Links.Add(NewLink);
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
  DirPath = FPaths::Combine(DirPath, TEXT("/../"));
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
  MeshRelativePath.RemoveFromStart(TEXT("model://"));
  TArray<FString> PackageParts;
  MeshRelativePath.ParseIntoArray(PackageParts, TEXT("/"));
  FString PackageName = PackageParts[0];


  FString PackagePath = GetROSPackagePath(PackageName);

  if(PackagePath.IsEmpty())
    {
      // Create mesh absolute path
      return DirPath + MeshRelativePath;
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("MeshPath %s"), *(PackagePath + MeshRelativePath));
      return PackagePath + MeshRelativePath;
    }
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


  //UPackage* Pkg = CreatePackage(NULL, *PackageName);
  UPackage* Pkg = CreatePackage(*PackageName);
  UStaticMesh* Mesh = RStaticMeshUtils::CreateStaticMesh(Pkg, PackageName, InShape, InParameters);
  CreateCollisionForMesh(Mesh, InShape);
  return Mesh;
}

void FSDFParser::GetROSPackagePaths()
{
  FString TempPath = FPlatformMisc::GetEnvironmentVariable(TEXT("ROS_PACKAGE_PATH"));
  if (!TempPath.IsEmpty())
    {
      ROSPackagePaths.Empty();
      TArray<FString> TempPathArray;
      TempPath.ParseIntoArray(TempPathArray, TEXT(":"));
      for(auto & Path : TempPathArray)
        {
          TArray<FString> TempPackageParts;
          Path.ParseIntoArray(TempPackageParts, TEXT("/"));
          FString PackageName = TempPackageParts.Top();
          if(!Path.Contains("opt"))
            {
              Path.RemoveFromEnd(PackageName);
              ROSPackagePaths.Add(PackageName, Path);
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("ROS default path %s"), *Path);
              ROSPackagePaths.Add(TEXT("ROS"), Path + TEXT("/"));
            }
        }
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("ROS_PACKAGE_PATH is empty or not set"));
    }
}

FString FSDFParser::GetROSPackagePath(const FString& InPackageName)
{
  if(ROSPackagePaths.Contains(InPackageName))
    {
      return ROSPackagePaths[InPackageName];
    }
  else
    {
      if(ROSPackagePaths.Contains(TEXT("ROS")))
        {
          FString TestPath = FPaths::Combine(ROSPackagePaths[TEXT("ROS")],  InPackageName);
          if(FPaths::DirectoryExists(TestPath))
            {
              return ROSPackagePaths[TEXT("ROS")];
            }
          else
            {
              UE_LOG(LogTemp, Error, TEXT("[%s] testpath %s does not exist"), *FString(__FUNCTION__), *InPackageName);
            }
        }
      UE_LOG(LogTemp, Error, TEXT("[%s] ROSPackage %s not found"), *FString(__FUNCTION__), *InPackageName);
      return FString();
    }
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
  //UPackage* Pkg = CreatePackage(NULL, *PackageName);
  UPackage* Pkg = CreatePackage(*PackageName);
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
