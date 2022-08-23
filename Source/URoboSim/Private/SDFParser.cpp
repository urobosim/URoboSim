// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "SDFParser.h"
#include "Conversions.h"
#include "XmlFile.h"
#include "GenericPlatform/GenericPlatformMisc.h"

// Default constructor
FSDFParser::FSDFParser() : AssetRegistryModule(
	FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry")))
{
	this->XmlFile = nullptr;
	this->bSDFLoaded = false;
	PackageDirs.AddUnique(TEXT("URoboSim"));
	PackageDirs.AddUnique(TEXT("Game"));
	GetROSPackagePaths();
}

// Constructor with load from path
FSDFParser::FSDFParser(const FString& InFilename) : AssetRegistryModule(
	FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry")))
{
	this->XmlFile = nullptr;
	this->bSDFLoaded = false;
	PackageDirs.AddUnique(TEXT("URoboSim"));
	PackageDirs.AddUnique(TEXT("Game"));
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
	XmlFile = new FXmlFile(InFilename, EConstructMethod::ConstructFromBuffer);

	// Check for valid sdf
	bSDFLoaded = IsValidSDF();

	// Set the model directory path
	SetDirectoryPath(InFilename);

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

	if(!XmlFile->IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SDFFile not valid"));
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
	if (!Name.IsEmpty())
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

void FSDFParser::ParsePlugin(const FXmlNode* InNode, USDFModel*& NewModel)
{
	USDFPlugin* NewPlugin = nullptr;
	// Get "name" from node attribute
	const FString Name = InNode->GetAttribute(TEXT("name"));
	NewPlugin = NewObject<USDFPlugin>(NewModel, FName(*Name));
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
		NewModel->Plugins.AddUnique(NewPlugin);
	}
}

// Parse <joint> node
void FSDFParser::ParseJoint(const FXmlNode* InNode, USDFModel*& NewModel)
{
	// Pointer to the new joint
	USDFJoint* NewJoint = nullptr;

	// Get "name" from node attribute
	const FString Name = InNode->GetAttribute(TEXT("name"));
	if (!Name.IsEmpty())
	{
		NewJoint = NewObject<USDFJoint>(NewModel, FName(*Name));
		NewJoint->Name = Name;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <joint> has no \"name\" attribute, added a default value.."),
		       *FString(__FUNCTION__), __LINE__);
		NewJoint = NewObject<USDFJoint>(NewModel/*, FName(TEXT("__default__"))*/);
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
			FString TempPoseRelativeTo = ChildNode->GetAttribute(TEXT("relative_to"));
			UE_LOG(LogTemp, Warning, TEXT("PoseRelativeTo %s"), *TempPoseRelativeTo);
			if (TempPoseRelativeTo.IsEmpty())
			{
				NewJoint->PoseRelativeTo = TEXT("Default");
			}
			else
			{
				NewJoint->PoseRelativeTo = TempPoseRelativeTo;
			}
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
	NewModel->Joints.Add(NewJoint);
}

// Parse <link> node
void FSDFParser::ParseLink(const FXmlNode* InNode, USDFModel*& NewModel)
{
	// Ptr to the new link
	USDFLink* NewLink = nullptr;

	// Get "name" from node attribute
	const FString Name = InNode->GetAttribute(TEXT("name"));
	if (!Name.IsEmpty())
	{
		NewLink = NewObject<USDFLink>(NewModel, FName(*Name));
		NewLink->Name = Name;
		CurrentLinkName = Name;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> has no \"name\" attribute, added a default value.."),
		       *FString(__FUNCTION__), __LINE__);
		NewLink = NewObject<USDFLink>(NewModel/*, FName(TEXT("__default__"))*/);
		NewLink->Name = TEXT("__default__");
	}

	// Iterate <link> child nodes
	for (const auto& ChildNode : InNode->GetChildrenNodes())
	{
		if (ChildNode->GetTag().Equals(TEXT("pose")))
		{
			FString TempPoseRelativeTo = ChildNode->GetAttribute(TEXT("relative_to"));
			if (TempPoseRelativeTo.IsEmpty())
			{
				NewLink->PoseRelativeTo = TEXT("Default");
			}
			else
			{
				NewLink->PoseRelativeTo = ChildNode->GetAttribute(TEXT("relative_to"));
			}
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

	// Add link to the data asset
	NewModel->Links.Add(NewLink);
}

// Parse <visual> node
void FSDFParser::ParseVisual(const FXmlNode* InNode, USDFLink*& NewLink)
{
	// Ptr to the new visual
	USDFVisual* NewVisual = nullptr;

	// Get "name" from node attribute
	const FString Name = InNode->GetAttribute(TEXT("name"));
	if (!Name.IsEmpty())
	{
		NewVisual = NewObject<USDFVisual>(NewLink, FName(*Name));
		NewVisual->Name = Name;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <link> has no \"name\" attribute, added a default value.."),
		       *FString(__FUNCTION__), __LINE__);
		NewVisual = NewObject<USDFVisual>(NewLink/*, FName(TEXT("__default__"))*/);
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
				ParseVisualChild(InNode, NewVisual);
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
	NewLink->Visuals.Add(NewVisual);
}

// Parse <visual> child node
void FSDFParser::ParseVisualChild(const FXmlNode* InNode, USDFVisual*& NewVisual)
{
	FName MeshName = GenerateMeshName(ESDFType::Visual, NewVisual->Name);
	FString NewDir = TEXT("Misc/");
	FString PackageName = GeneratePackageName(MeshName, NewDir);
	//TODO check existance of package
	NewVisual->Geometry->Mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(),
																		nullptr, *PackageName));
}

// Parse <collision> node
void FSDFParser::ParseCollision(const FXmlNode* InNode, USDFLink*& NewLink)
{
	// Ptr to the new collision
	USDFCollision* NewCollision = nullptr;

	// Get "name" from node attribute
	const FString Name = InNode->GetAttribute(TEXT("name"));
	if (!Name.IsEmpty())
	{
		NewCollision = NewObject<USDFCollision>(NewLink, FName(*Name));
		NewCollision->Name = Name;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <collision> has no \"name\" attribute, added a default value.."),
		       *FString(__FUNCTION__), __LINE__);
		NewCollision = NewObject<USDFCollision>(NewLink/*, FName(TEXT("__default__"))*/);
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
			if (NewCollision->Geometry->Type == ESDFGeometryType::Box ||
				NewCollision->Geometry->Type == ESDFGeometryType::Cylinder ||
				NewCollision->Geometry->Type == ESDFGeometryType::Sphere)
			{
				ParseCollisionChild(InNode, NewCollision);
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
	NewLink->Collisions.Add(NewCollision);
}

void FSDFParser::ParseCollisionChild(const FXmlNode* InNode, USDFCollision*& NewCollision)
{
	FName MeshName = GenerateMeshName(ESDFType::Collision, NewCollision->Name);
	FString NewDir = TEXT("Misc/");
	FString PackageName = GeneratePackageName(MeshName, NewDir);
	NewCollision->Geometry->Mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(),
																		nullptr, *PackageName));
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
			OutGeometry->Mesh = GetMesh(OutGeometry->Uri, Type);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s][%d] <geometry> <mesh> child <%s> not supported, ignored.."),
			       *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
			continue;
		}
	}
}

UStaticMesh* FSDFParser::GetMesh(const FString& Uri, ESDFType Type)
{
	FString MeshRelativePath = Uri;
	FString MeshNameTemp = FPaths::GetBaseFilename(Uri);

	FName MeshName = GenerateMeshName(Type, MeshNameTemp);
	FString PackageName = GeneratePackageName(MeshName, MeshRelativePath);
	UE_LOG(LogTemp, Log, TEXT("MeshName %s PackageName %s"), *MeshNameTemp, *PackageName);
	return Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *PackageName));
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
	TArray<FString> PackageParts;
	Uri.ParseIntoArray(PackageParts, TEXT("/"));
	FString PackageName = PackageParts[0];


	FString PackagePath = GetROSPackagePath(PackageName);

	if (PackagePath.IsEmpty())
	{
		// Create mesh absolute path
		return DirPath + Uri;
	}
	else
	{
		return PackagePath + Uri;
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

FString FSDFParser::GeneratePackageName(FName MeshName, FString InPackagePath)
{
	FString PackageName = "";
	FString Reason = "";
	// FString NewDir = DataAsset->GetNewer()->GetPathName() + "/" + CurrentLinkName;

	// Remove package name prefix
	InPackagePath.RemoveFromStart(TEXT("model://"));
	int32 Pos = InPackagePath.Find(TEXT("/"), ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	//Check if Package is already in URoboSim or Game
	for(auto& PackageDir : PackageDirs)
	{
		FString NewDir = TEXT("/") + PackageDir + TEXT("/Packages/") + InPackagePath.Left(Pos);
		if (!FPackageName::TryConvertFilenameToLongPackageName(NewDir + "/" + MeshName.ToString(), PackageName, &Reason))
		{
			UE_LOG(LogTemp, Error, TEXT("Packacke name invlaide because : %s"), *Reason);
		}

		if(FPackageName::DoesPackageExist(PackageName))
		{
			return PackageName;
		}
	}

	//Not found in URoboSim or Game --> Add new package to ContentDir
	FString ContentDir = DataAsset->GetPathName();
	ContentDir.RightChopInline(1);
	TArray<FString> PathPart;
	if(ContentDir.ParseIntoArray(PathPart, TEXT("/")) > 0)
	{
		ContentDir = PathPart[0];
	}
	else
	{
		ContentDir = TEXT("Game");
	}
	UE_LOG(LogTemp, Log, TEXT("Content Dir : %s"), *ContentDir);

	FString NewDir = TEXT("/") + ContentDir + TEXT("/Packages/") + InPackagePath.Left(Pos);
	if (!FPackageName::TryConvertFilenameToLongPackageName(NewDir + "/" + MeshName.ToString(), PackageName, &Reason))
	{
		UE_LOG(LogTemp, Error, TEXT("Packacke name invlaide because : %s"), *Reason);
	}

	AssetRegistryModule.Get().AddPath(NewDir);
	return PackageName;
}


void FSDFParser::GetROSPackagePaths()
{
	FString TempPath = FPlatformMisc::GetEnvironmentVariable(TEXT("ROS_PACKAGE_PATH"));
	if (!TempPath.IsEmpty())
	{
		ROSPackagePaths.Empty();
		TArray<FString> TempPathArray;
		TempPath.ParseIntoArray(TempPathArray, TEXT(":"));
		for (auto& Path : TempPathArray)
		{
			TArray<FString> TempPackageParts;
			Path.ParseIntoArray(TempPackageParts, TEXT("/"));
			FString PackageName = TempPackageParts.Top();
			if (!Path.Contains("opt"))
			{
				Path.RemoveFromEnd(PackageName);
				ROSPackagePaths.Add(PackageName, Path);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("ROS default path %s"), *Path);
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
	if (ROSPackagePaths.Contains(InPackageName))
	{
		return ROSPackagePaths[InPackageName];
	}
	else
	{
		if (ROSPackagePaths.Contains(TEXT("ROS")))
		{
			FString TestPath = FPaths::Combine(ROSPackagePaths[TEXT("ROS")], InPackageName);
			if (FPaths::DirectoryExists(TestPath))
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
