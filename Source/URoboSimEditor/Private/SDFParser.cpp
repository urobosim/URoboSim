// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "SDFParser.h"
#include "Conversions.h"
#include "Paths.h"
#include "XmlFile.h"
#include "Factories/FbxFactory.h"
#include "RStaticMeshEditUtils.h"

//// Default constructor
//FSDFParser::FSDFParser() : AssetRegistryModule(FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry")))
//{
//    UE_LOG(LogTemp, Warning, TEXT("CreateFSDFParser--> Construtor"));
//    this->XmlFile=nullptr;
//    this->bSDFLoaded=false;
//}

//// Constructor with load from path
//FSDFParser::FSDFParser(const FString& InFilename) : AssetRegistryModule(FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry")))
//{
//    this->XmlFile=nullptr;
//    this->bSDFLoaded=false;
//  LoadSDF(InFilename);
//}

//// Destructor
//FSDFParser::~FSDFParser()
//{
//  Clear();
//}



// Create data asset and parse sdf data into it
//USDFDataAsset* FSDFParser::ParseToNewDataAsset(UObject* InParent, FName InName, EObjectFlags InFlags)
//{
//  if (!bSDFLoaded)
//    {
//      return nullptr;
//    }

//  // Create a new SDFDataAsset
//  DataAsset = NewObject<USDFDataAsset>(InParent, InName, InFlags);

//  // Parse sdf data and fill the data asset
//  ParseSDF();

//  return DataAsset;
//}



//// Load sdf from file
//bool FSDFParser::LoadSDF(const FString& InFilename)
//{
//  UE_LOG(LogTemp, Log, TEXT("From SDF-Parser.LoadSDF before secondtime LoadModuleCheck"));
//  AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
//  UE_LOG(LogTemp, Log, TEXT("From SDF-Parser.LoadSDF after secondtime LoadModuleCheck"));
//  // Make sure parser is clean
//  Clear();

//  // Load xml file
//  XmlFile = new FXmlFile(InFilename);

//  // Check for valid sdf
//  bSDFLoaded = IsValidSDF();

//  // Set the model directory path
//  SetDirectoryPath(InFilename);

//  UE_LOG(LogTemp, Log, TEXT("From SDF-Parser.LoadSDF before creating the FBXFactory"));
//  // Create fbx factory for loading the meshes
//  FbxFactory = NewObject<UFbxFactory>(UFbxFactory::StaticClass());
//  UE_LOG(LogTemp, Log, TEXT("From SDF-Parser.LoadSDF after creating the FBXFactory"));

//  // If options are to be offered before loading.
//  FbxFactory->EnableShowOption();

//  return bSDFLoaded;
//}


//// Parse <visual> node
//void FSDFParser::ParseVisual(const FXmlNode* InNode, USDFLink*& OutLink)
//{
//  // Ptr to the new visual
//  USDFVisual* NewVisual = nullptr;

//  // Get "name" from node attribute
//  const FString Name = InNode->GetAttribute(TEXT("name"));
//  if (!Name.IsEmpty())
//    {
//      NewVisual = NewObject<USDFVisual>(OutLink, FName(*Name));
//      NewVisual->Name = Name;
//    }
//  else
//    {
//      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> has no \"name\" attribute, added a default value.."),
//             *FString(__FUNCTION__), __LINE__);
//      NewVisual = NewObject<USDFVisual>(OutLink/*, FName(TEXT("__default__"))*/);
//      NewVisual->Name = TEXT("__default__");
//    }

//  // Iterate <visual> child nodes
//  for (const auto& ChildNode : InNode->GetChildrenNodes())
//    {
//      if (ChildNode->GetTag().Equals(TEXT("pose")))
//        {
//          NewVisual->Pose = PoseContentToFTransform(ChildNode->GetContent());
//        }
//      else if (ChildNode->GetTag().Equals(TEXT("geometry")))
//        {
//          ParseGeometry(ChildNode, NewVisual->Geometry, ESDFType::Visual);
//          if(NewVisual->Geometry->Type == ESDFGeometryType::Box ||
//             NewVisual->Geometry->Type == ESDFGeometryType::Cylinder ||
//             NewVisual->Geometry->Type == ESDFGeometryType::Sphere)
//            {
//              NewVisual->Geometry->Mesh = CreateMesh(ESDFType::Visual, NewVisual->Geometry->Type, Name, RStaticMeshUtils::GetGeometryParameter(NewVisual->Geometry));
//            }
//        }
//      else
//        {
//          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> <visual> child <%s> not supported, ignored.."),
//                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
//          continue;
//        }
//    }

//  // Add visual to array
//  OutLink->Visuals.Add(NewVisual);
//}

//// Parse <collision> node
//void FSDFParser::ParseCollision(const FXmlNode* InNode, USDFLink*& OutLink)
//{
//  // Ptr to the new collision
//  USDFCollision* NewCollision = nullptr;

//  // Get "name" from node attribute
//  const FString Name = InNode->GetAttribute(TEXT("name"));
//  if (!Name.IsEmpty())
//    {
//      NewCollision = NewObject<USDFCollision>(OutLink, FName(*Name));
//      NewCollision->Name = Name;
//    }
//  else
//    {
//      UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <collision> has no \"name\" attribute, added a default value.."),
//             *FString(__FUNCTION__), __LINE__);
//      NewCollision = NewObject<USDFCollision>(OutLink/*, FName(TEXT("__default__"))*/);
//      NewCollision->Name = TEXT("__default__");
//    }

//  // Iterate <collision> child nodes
//  for (const auto& ChildNode : InNode->GetChildrenNodes())
//    {
//      if (ChildNode->GetTag().Equals(TEXT("pose")))
//        {
//          NewCollision->Pose = PoseContentToFTransform(ChildNode->GetContent());
//        }
//      else if (ChildNode->GetTag().Equals(TEXT("geometry")))
//        {
//          ParseGeometry(ChildNode, NewCollision->Geometry, ESDFType::Collision);
//          if(NewCollision->Geometry->Type == ESDFGeometryType::Box ||
//             NewCollision->Geometry->Type == ESDFGeometryType::Cylinder ||
//             NewCollision->Geometry->Type == ESDFGeometryType::Sphere)
//            {
//              NewCollision->Geometry->Mesh = CreateMesh(ESDFType::Collision, NewCollision->Geometry->Type, Name, RStaticMeshUtils::GetGeometryParameter(NewCollision->Geometry));
//              // RStaticMeshUtils::CreateStaticMeshThroughBrush(OutLink,NewCollision);
//            }
//        }
//      else
//        {
//          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <inertial> child <%s> not supported, ignored.."),
//                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
//          continue;
//        }
//    }

//  // Add collision to array
//  OutLink->Collisions.Add(NewCollision);
//}






///* Begin helper functions */
//// Fix file path
//void FSDFParser::SetDirectoryPath(const FString& InFilename)
//{
//  // Replace back slash to forward slash in the path
//  // D:\Models\MyModelName\model.sdf --> D:/Models/MyModelName/model.sdf
//  DirPath = InFilename.Replace(TEXT("\\"), TEXT("/"));

//  // Remove filename from path --> D:/Models/MyModelName
//  DirPath = FPaths::GetPath(DirPath);

//  // TODO rather cut uppermost from URI
//  // One dir up --> D:/Models
//  DirPath = FPaths::Combine(DirPath, TEXT("/.."));
//  FPaths::CollapseRelativeDirectories(DirPath);
//}

//// Get mesh absolute path
//FString FSDFParser::GetMeshAbsolutePath(const FString& Uri)
//{
//  // Create mesh relative path, add .fbx extension
//  FString MeshRelativePath = Uri;
//  if (!MeshRelativePath.EndsWith(".fbx"))
//    {
//      MeshRelativePath = FPaths::ChangeExtension(MeshRelativePath, TEXT("fbx"));
//    }
//  // Remove package name prefix
//  MeshRelativePath.RemoveFromStart(TEXT("model:/"));

//  // Create mesh absolute path
//  return DirPath + MeshRelativePath;
//}

//FName FSDFParser::GenerateMeshName(ESDFType InType, FString InName)
//{
//  FName MeshName;
//  if (InType == ESDFType::Collision)
//    {
//      MeshName = FName(*(TEXT("SM_") + InName + TEXT("_C")));
//    }
//  else if (InType == ESDFType::Visual)
//    {
//      MeshName = FName(*(TEXT("SM_") + InName + TEXT("_V")));
//    }
//  return MeshName;
//}

//FString FSDFParser::GeneratePackageName(FName MeshName)
//{
//  FString PackageName = "";
//  FString Reason = "";
//  FString NewDir = DataAsset->GetOuter()->GetPathName() + "/" + CurrentLinkName;
//  if(!FPackageName::TryConvertFilenameToLongPackageName(NewDir + "/" + MeshName.ToString() , PackageName, &Reason))
//    {
//      UE_LOG(LogTemp, Error, TEXT("Packacke name invlaide because : %s"), *Reason);
//    }
//  AssetRegistryModule.Get().AddPath(NewDir);
//  return PackageName;
//}

//UStaticMesh* FSDFParser::CreateMesh(ESDFType InType, ESDFGeometryType InShape, FString InName, TArray<float> InParameters)
//{
//  // FString Path = "";
//  FName MeshName = GenerateMeshName(InType, InName);
//  FString PackageName = GeneratePackageName(MeshName);


//  UPackage* Pkg = CreatePackage(NULL, *PackageName);
//  UStaticMesh* Mesh = RStaticMeshUtils::CreateStaticMesh(Pkg, PackageName, InShape, InParameters);
//  CreateCollisionForMesh(Mesh, InShape);
//  return Mesh;
//}

//// Import .fbx meshes from data asset
//UStaticMesh* FSDFParser::ImportMesh(const FString& Uri, ESDFType Type)
//{
//  FString MeshAbsolutePath = GetMeshAbsolutePath(Uri);
//  if (!FPaths::FileExists(MeshAbsolutePath))
//    {
//      UE_LOG(LogTemp, Error, TEXT("[%s] Could not find %s"), *FString(__FUNCTION__), *MeshAbsolutePath);
//      return nullptr;
//    }

//  // Get mesh name

//  FString MeshNameTemp = FPaths::GetBaseFilename(MeshAbsolutePath);
//  FName MeshName = GenerateMeshName(Type, MeshNameTemp);

//  // Import mesh
//  bool bOperationCancelled = false;

//  FString PackageName = GeneratePackageName(MeshName);
//  UPackage* Pkg = CreatePackage(NULL, *PackageName);
//  UObject* MeshObj = FbxFactory->ImportObject(
//                                              UStaticMesh::StaticClass(), Pkg, MeshName, RF_Transactional | RF_Standalone | RF_Public, MeshAbsolutePath, nullptr, bOperationCancelled);

//  // If import has been cancelled
//  if (bOperationCancelled)
//    {
//      return nullptr;
//    }

//  FAssetRegistryModule::AssetCreated(MeshObj);

//  // Mark outer dirty
//  if (DataAsset->GetOuter())
//    {
//      DataAsset->GetOuter()->MarkPackageDirty();
//    }
//  else
//    {
//      DataAsset->MarkPackageDirty();
//    }
//  // Mark mesh dirty
//  if (MeshObj)
//    {
//      MeshObj->MarkPackageDirty();
//    }

//  // Return the cast result
//  return Cast<UStaticMesh>(MeshObj);
//}

//bool FSDFParser::CreateCollisionForMesh(UStaticMesh* OutMesh, ESDFGeometryType Type)
//{
//  switch(Type)
//    {
//    case ESDFGeometryType::None :
//      return false;
//    case ESDFGeometryType::Mesh :
//      return true;
//    case ESDFGeometryType::Box :
//      RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopX10);
//      return true;
//    case ESDFGeometryType::Cylinder :
//      RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopZ10);
//      return true;
//    case ESDFGeometryType::Sphere :
//      RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopX10);
//      return true;
//    default :
//      UE_LOG(LogTemp, Error, TEXT("GeometryType not supportet for %s."), *OutMesh->GetName());
//      return false;
//    }
//}

//USDFCollision* FSDFParser::CreateVirtualCollision(USDFLink* OutLink)
//{
//  USDFCollision* NewCollision = NewObject<USDFCollision>(OutLink, FName(*CurrentLinkName));
//  NewCollision->Name = CurrentLinkName;
//  NewCollision->Pose = FTransform();
//  NewCollision->Geometry = NewObject<USDFGeometry>(NewCollision);
//  NewCollision->Geometry->Type = ESDFGeometryType::Box;
//  NewCollision->Geometry->Size = FVector(0.5f, 0.5f, 0.5f);
//  NewCollision->Geometry->Mesh = CreateMesh(ESDFType::Collision, ESDFGeometryType::Box, CurrentLinkName, RStaticMeshUtils::GetGeometryParameter(NewCollision->Geometry));
//  return NewCollision;
//}
