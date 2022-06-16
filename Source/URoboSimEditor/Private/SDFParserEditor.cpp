// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "SDFParserEditor.h"
#include "Factories/FbxFactory.h"
#include "RStaticMeshEditUtils.h"
#include "XmlFile.h"


// Default constructor
FSDFParserEditor::FSDFParserEditor()
{
    this->XmlFile=nullptr;
    this->bSDFLoaded=false;
    GetROSPackagePaths();
}

FSDFParserEditor::FSDFParserEditor(const FString& InFilename)  
{
  this->XmlFile=nullptr;
  this->bSDFLoaded=false;
  GetROSPackagePaths();
  LoadSDF(InFilename);
}

// Load sdf from file
bool FSDFParserEditor::LoadSDF(const FString& InFilename)
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


// Import .fbx meshes from data asset
UStaticMesh* FSDFParserEditor::ImportMesh(const FString& Uri, ESDFType Type)
{
  FString MeshRelativePath = Uri;
  if (!MeshRelativePath.EndsWith(".fbx"))
    {
      MeshRelativePath = FPaths::ChangeExtension(MeshRelativePath, TEXT("fbx"));
    }
  // Remove package name prefix
  MeshRelativePath.RemoveFromStart(TEXT("model://"));
  
  FString MeshAbsolutePath = GetMeshAbsolutePath(MeshRelativePath);
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

  FString PackageName = GeneratePackageName(MeshName, MeshRelativePath);
#if ENGINE_MINOR_VERSION < 27 || ENGINE_MAJOR_VERSION >4
  UPackage* Pkg = CreatePackage(NULL, *PackageName);
#else
  UPackage* Pkg = CreatePackage(*PackageName);
#endif //Version
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


bool FSDFParserEditor::CreateCollisionForMesh(UStaticMesh* OutMesh, ESDFGeometryType Type)
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

USDFCollision* FSDFParserEditor::CreateVirtualCollision(USDFLink* OutLink)
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

void FSDFParserEditor::Clear()
{
  FSDFParser::Clear();
  if (bSDFLoaded)
    {
      FbxFactory = nullptr;
    }
}

UStaticMesh* FSDFParserEditor::CreateMesh(ESDFType InType, ESDFGeometryType InShape, FString InName, TArray<float> InParameters)
{
  // FString Path = "";
  FName MeshName = GenerateMeshName(InType, InName);
  FString NewDir = TEXT("Misc/");
  FString PackageName = GeneratePackageName(MeshName,NewDir);


#if ENGINE_MINOR_VERSION < 27 || ENGINE_MAJOR_VERSION >4
  UPackage* Pkg = CreatePackage(NULL, *PackageName);
#else
  UPackage* Pkg = CreatePackage(*PackageName);
#endif //Version

  
  UStaticMesh* Mesh = RStaticMeshUtils::CreateStaticMesh(Pkg, PackageName, InShape, InParameters);
  CreateCollisionForMesh(Mesh, InShape);
  
  return Mesh;
}

void FSDFParserEditor::ParseLink(const FXmlNode* InNode, USDFModel*& OutModel)
{
  FSDFParser::ParseLink(InNode, OutModel);
  if(OutModel->Links.Last()->Collisions.Num() == 0)
    {
      USDFCollision* Collision = CreateVirtualCollision(OutModel->Links.Last());
    
      if(Collision)
        {
          OutModel->Links.Last()->Collisions.Add(Collision);
        }
      else
        {
          UE_LOG(LogTemp, Error, TEXT("Creation of Virtual Link %s failed"), *CurrentLinkName);
        }
    }
}

void FSDFParserEditor::ParseVisualChild(const FXmlNode* InNode, USDFVisual*& OutVisual)
{
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          OutVisual->Pose = PoseContentToFTransform(ChildNode->GetContent());
        }
      else if (ChildNode->GetTag().Equals(TEXT("geometry")))
        {
          ParseGeometry(ChildNode, OutVisual->Geometry, ESDFType::Visual);
          if(OutVisual->Geometry->Type == ESDFGeometryType::Box ||
             OutVisual->Geometry->Type == ESDFGeometryType::Cylinder ||
             OutVisual->Geometry->Type == ESDFGeometryType::Sphere)
            {
              OutVisual->Geometry->Mesh = CreateMesh(ESDFType::Visual, OutVisual->Geometry->Type, OutVisual->Name, RStaticMeshUtils::GetGeometryParameter(OutVisual->Geometry));
            }
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> <visual> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
}
void FSDFParserEditor::ParseCollisionChild(const FXmlNode* InNode, USDFCollision*& OutCollision)
{
  for (const auto& ChildNode : InNode->GetChildrenNodes())
    {
      if (ChildNode->GetTag().Equals(TEXT("pose")))
        {
          OutCollision->Pose = PoseContentToFTransform(ChildNode->GetContent());

        }
      else if (ChildNode->GetTag().Equals(TEXT("geometry")))
        {
          ParseGeometry(ChildNode, OutCollision->Geometry, ESDFType::Collision);
          if(OutCollision->Geometry->Type == ESDFGeometryType::Box ||
             OutCollision->Geometry->Type == ESDFGeometryType::Cylinder ||
             OutCollision->Geometry->Type == ESDFGeometryType::Sphere)
            {
              OutCollision->Geometry->Mesh = CreateMesh(ESDFType::Collision, OutCollision->Geometry->Type, OutCollision->Name, RStaticMeshUtils::GetGeometryParameter(OutCollision->Geometry));
            }
        }
      else
        {
          UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <inertial> child <%s> not supported, ignored.."),
                 *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
          continue;
        }
    }
  
}

// Parse <geometry> <mesh> node
void FSDFParserEditor::ParseGeometryMesh(const FXmlNode* InNode, USDFGeometry*& OutGeometry, ESDFType Type)
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
