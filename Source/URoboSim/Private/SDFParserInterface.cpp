// Fill out your copyright notice in the Description page of Project Settings.

#include "SDFParserInterface.h"
#include "Conversions.h"

DEFINE_LOG_CATEGORY_STATIC(LogSDFParserInterface, Log, All);

// Clear parser
void ISDFParserInterface::Clear()
{
  if (XmlFile)
  {
    delete XmlFile;
    XmlFile = nullptr;
  }
  DataAsset = nullptr;
  bSDFLoaded = false;
}

// Parse <sdf> node
void ISDFParserInterface::ParseSDF()
{
  // Get "version" from node attribute
  const FString SDFVersion = XmlFile->GetRootNode()->GetAttribute(TEXT("version"));
  if (!SDFVersion.IsEmpty())
  {
    DataAsset->Version = SDFVersion;
  }
  else
  {
    UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <sdf> has no \"version\" attribute, added a default value.."),
           *FString(__FUNCTION__), __LINE__)
    DataAsset->Version = TEXT("__default__");
  }

  // Iterate <sdf> child nodes
  for (const auto &ChildNode : XmlFile->GetRootNode()->GetChildrenNodes())
  {
    // Check for <model>
    if (ChildNode->GetTag().Equals(TEXT("model")))
    {
      ParseModel(ChildNode);
    }
    else
    {
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <sdf> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag())
    }
  }
}

// Parse <model> node
void ISDFParserInterface::ParseModel(const FXmlNode *InNode)
{
  // Ptr to the new model
  USDFModel *NewModel = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty())
  {
    NewModel = NewObject<USDFModel>(DataAsset, FName(*Name));
  }
  else
  {
    UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <model> has no \"name\" attribute, added a default value.."),
           *FString(__FUNCTION__), __LINE__)
    NewModel = NewObject<USDFModel>(DataAsset);
  }

  // Iterate <model> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
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
    else if (ChildNode->GetTag().Equals(TEXT("bSelfColide")))
    {
      NewModel->bSelfColide = ChildNode->GetContent().ToBool();
    }
    else
    {
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <model> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag())
    }
  }

  // Add model to the data asset
  DataAsset->Model = NewModel;
}

// Parse <link> node
void ISDFParserInterface::ParseLink(const FXmlNode *InNode, USDFModel *&OutModel)
{
  // Ptr to the new link
  USDFLink *NewLink = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty())
  {
    NewLink = NewObject<USDFLink>(OutModel, FName(*Name));
  }
  else
  {
    UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <link> has no \"name\" attribute, added a default value.."),
           *FString(__FUNCTION__), __LINE__)
    NewLink = NewObject<USDFLink>(OutModel);
  }

  // Iterate <link> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
  {
    if (ChildNode->GetTag().Equals(TEXT("pose")))
    {
      NewLink->Pose = PoseContentToFTransform(ChildNode->GetContent());
    }
    else if (ChildNode->GetTag().Equals(TEXT("inertial")))
    {
      ParseLinkInertial(ChildNode, NewLink);
    }
    else if (ChildNode->GetTag().Equals(TEXT("inertial")))
    {
      ParseLinkInertialInertia(ChildNode, NewLink);
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
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][%d] <link> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag())
    }
  }

  // Create default collision for link which has inertia
  if (NewLink->Collisions.Num() == 0 && !NewLink->Inertial->Inertia.IsNearlyZero())
  {
    UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][%d] <link> %s does not have any collision meshes, create one default.."),
           *FString(__FUNCTION__), __LINE__, *NewLink->GetName())

    NewLink->Collisions.Add(NewObject<USDFCollision>(NewLink, TEXT("DefaultCollision")));
    USDFGeometryBox *NewGeometryBox = NewObject<USDFGeometryBox>(NewLink->Collisions[0], TEXT("DefaultBox"));
    NewGeometryBox->Type = ESDFGeometryType::Box;
    NewGeometryBox->Size = FVector(0.001f);
    NewLink->Collisions[0]->Geometry = NewGeometryBox;
  }

  // Add link to the data asset
  OutModel->Links.Add(NewLink);
}

// Parse <link> <inertial> node
void ISDFParserInterface::ParseLinkInertial(const FXmlNode *InNode, USDFLink *&OutLink)
{
  // Iterate <link> <inertial> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
  {
    if (ChildNode->GetTag().Equals(TEXT("mass")))
    {
      OutLink->Inertial->Mass = FCString::Atof(*ChildNode->GetContent());
    }
    else if (ChildNode->GetTag().Equals(TEXT("pose")))
    {
      OutLink->Inertial->Pose = PoseContentToFTransform(ChildNode->GetContent());
    }
    else if (ChildNode->GetTag().Equals(TEXT("inertia")))
    {
      ParseLinkInertialInertia(ChildNode, OutLink);
    }
    else
    {
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <inertial> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
    }
  }
}

// Parse <link> <inertial> <inertia> node
void ISDFParserInterface::ParseLinkInertialInertia(const FXmlNode *InNode, USDFLink *&OutLink)
{
  // Iterate <link> <inertial> <inertia> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
  {
    if (ChildNode->GetTag().Equals(TEXT("ixx")))
    {
      OutLink->Inertial->Inertia.X = FCString::Atof(*ChildNode->GetContent());
    }
    else if (ChildNode->GetTag().Equals(TEXT("iyy")))
    {
      OutLink->Inertial->Inertia.Y = FCString::Atof(*ChildNode->GetContent());
    }
    else if (ChildNode->GetTag().Equals(TEXT("izz")))
    {
      OutLink->Inertial->Inertia.Z = FCString::Atof(*ChildNode->GetContent());
    }
    else
    {
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <inertial> <inertia> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
    }
  }
}

// Parse <visual> node
void ISDFParserInterface::ParseVisual(const FXmlNode *InNode, USDFLink *&OutLink)
{
  // Ptr to the new visual
  USDFVisual *NewVisual = nullptr;
  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty())
  {
    NewVisual = NewObject<USDFVisual>(OutLink, FName(*Name));
  }
  else
  {
    UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <link> has no \"name\" attribute, added a default value.."),
           *FString(__FUNCTION__), __LINE__);
    NewVisual = NewObject<USDFVisual>(OutLink);
  }

  // Iterate <visual> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
  {
    if (ChildNode->GetTag().Equals(TEXT("pose")))
    {
      NewVisual->Pose = PoseContentToFTransform(ChildNode->GetContent());
    }
    else if (ChildNode->GetTag().Equals(TEXT("geometry")))
    {
      ParseVisualGeometry(ChildNode, NewVisual);
    }
    else
    {
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <link> <visual> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag())
    }
  }

  // Add visual to array
  OutLink->Visuals.Add(NewVisual);
}

// Parse <collision> node
void ISDFParserInterface::ParseCollision(const FXmlNode *InNode, USDFLink *&OutLink)
{
  // Ptr to the new collision
  USDFCollision *NewCollision = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty())
  {
    NewCollision = NewObject<USDFCollision>(OutLink, FName(*Name));
  }
  else
  {
    UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <collision> has no \"name\" attribute, added a default value.."),
           *FString(__FUNCTION__), __LINE__);
    NewCollision = NewObject<USDFCollision>(OutLink);
  }

  // Iterate <collision> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
  {
    if (ChildNode->GetTag().Equals(TEXT("pose")))
    {
      NewCollision->Pose = PoseContentToFTransform(ChildNode->GetContent());
    }
    else if (ChildNode->GetTag().Equals(TEXT("geometry")))
    {
      ParseCollisionGeometry(ChildNode, NewCollision);
    }
    else
    {
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <inertial> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
    }
  }

  // Add collision to array
  OutLink->Collisions.Add(NewCollision);
}

// Parse <visual> <geometry> node
void ISDFParserInterface::ParseVisualGeometry(const FXmlNode *InNode, USDFVisual *&OutVisual)
{
  ParseGeometry(InNode, OutVisual->Geometry);
}

// Parse <collision> <geometry> node
void ISDFParserInterface::ParseCollisionGeometry(const FXmlNode *InNode, USDFCollision *&OutCollision)
{
  ParseGeometry(InNode, OutCollision->Geometry);
}

// Parse <geometry> node
void ISDFParserInterface::ParseGeometry(const FXmlNode *InNode, USDFGeometry *&OutGeometry)
{
  // Iterate <geometry> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
  {
    if (ChildNode->GetTag().Equals(TEXT("mesh")))
    {
      ParseGeometryMesh(ChildNode, OutGeometry);
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
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <geometry> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag());
    }
  }
}

// Parse <geometry> <mesh> node
void ISDFParserInterface::ParseGeometryMesh(const FXmlNode *InNode, USDFGeometry *&OutGeometry)
{
  USDFGeometryMesh *NewGeometryMesh = NewObject<USDFGeometryMesh>(OutGeometry, TEXT("Mesh"));
  NewGeometryMesh->Type = ESDFGeometryType::Mesh;
  // Iterate <geometry> <mesh> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
  {
    if (ChildNode->GetTag().Equals(TEXT("uri")))
    {
      // Set Uri as the relative path from the asset to the mesh uasset
      NewGeometryMesh->Uri = ChildNode->GetContent();
      OutGeometry = NewGeometryMesh;
    }
    else if (ChildNode->GetTag().Equals(TEXT("scale")))
    {
      // Set Uri as the relative path from the asset to the mesh uasset
      NewGeometryMesh->Scale = ScaleContentToFVector(ChildNode->GetContent());
      OutGeometry = NewGeometryMesh;
    }
    else
    {
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <geometry> <mesh> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag())
    }
  }
}

// Parse <geometry> <box> node
void ISDFParserInterface::ParseGeometryBox(const FXmlNode *InNode, USDFGeometry *&OutGeometry)
{
  USDFGeometryBox *NewGeometryBox = NewObject<USDFGeometryBox>(OutGeometry, TEXT("MeshBox"));
  NewGeometryBox->Type = ESDFGeometryType::Box;
  // Iterate <geometry> <box> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
  {
    if (ChildNode->GetTag().Equals(TEXT("size")))
    {
      NewGeometryBox->Size = SizeToFVector(ChildNode->GetContent());
      OutGeometry = NewGeometryBox;
    }
    else
    {
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <geometry> <box> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag())
    }
  }
}

// Parse <geometry> <cylinder> node
void ISDFParserInterface::ParseGeometryCylinder(const FXmlNode *InNode, USDFGeometry *&OutGeometry)
{
  USDFGeometryCylinder *NewGeometryCylinder = NewObject<USDFGeometryCylinder>(OutGeometry, TEXT("MeshCylinder"));
  NewGeometryCylinder->Type = ESDFGeometryType::Cylinder;
  // Iterate <geometry> <cylinder> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
  {
    if (ChildNode->GetTag().Equals(TEXT("radius")))
    {
      NewGeometryCylinder->Radius = FConversions::MToCm(FCString::Atof(*ChildNode->GetContent()));
      OutGeometry = NewGeometryCylinder;
    }
    else if (ChildNode->GetTag().Equals(TEXT("length")))
    {
      NewGeometryCylinder->Length = FConversions::MToCm(FCString::Atof(*ChildNode->GetContent()));
      OutGeometry = NewGeometryCylinder;
    }
    else
    {
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <geometry> <cylinder> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag())
    }
  }
}

// Parse <geometry> <sphere> node
void ISDFParserInterface::ParseGeometrySphere(const FXmlNode *InNode, USDFGeometry *&OutGeometry)
{
  USDFGeometrySphere *NewGeometrySphere = NewObject<USDFGeometrySphere>(OutGeometry, TEXT("MeshSphere"));
  NewGeometrySphere->Type = ESDFGeometryType::Sphere;
  // Iterate <geometry> <sphere> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
  {
    if (ChildNode->GetTag().Equals(TEXT("radius")))
    {
      NewGeometrySphere->Radius = FConversions::MToCm(FCString::Atof(*ChildNode->GetContent()));
      OutGeometry = NewGeometrySphere;
    }
    else
    {
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <geometry> <sphere> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag())
    }
  }
}

// Parse <joint> node
void ISDFParserInterface::ParseJoint(const FXmlNode *InNode, USDFModel *&OutModel)
{
  // Pointer to the new joint
  USDFJoint *NewJoint = nullptr;

  // Get "name" from node attribute
  const FString Name = InNode->GetAttribute(TEXT("name"));
  if (!Name.IsEmpty())
  {
    NewJoint = NewObject<USDFJoint>(OutModel, FName(*Name));
  }
  else
  {
    UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][%d] <joint> has no \"name\" attribute, added a default value.."),
           *FString(__FUNCTION__), __LINE__)
    NewJoint = NewObject<USDFJoint>(OutModel);
  }

  // Get "type" from node attribute
  const FString Type = InNode->GetAttribute(TEXT("type"));
  if (!Name.IsEmpty())
  {
    NewJoint->Type = Type;
  }
  else
  {
    UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <joint> has no \"type\" attribute, added a default value.."),
           *FString(__FUNCTION__), __LINE__)
  }

  // Iterate <joint> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
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
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <joint> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag())
    }
  }

  // Add joint to the data asset
  OutModel->Joints.Add(NewJoint);
}

// Parse <joint> <axis> node
void ISDFParserInterface::ParseJointAxis(const FXmlNode *InNode, USDFJoint *&OutJoint)
{
  // Iterate <joint> <axis> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
  {
    if (ChildNode->GetTag().Equals(TEXT("xyz")))
    {
      OutJoint->Axis->Xyz = XyzToFVector(ChildNode->GetContent());
    }
    else if (ChildNode->GetTag().Equals(TEXT("initial_position")))
    {
      OutJoint->Axis->InitialPosition = FCString::Atof(*ChildNode->GetContent());
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
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <joint> <axis> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag())
    }
  }
}

// Parse <joint> <axis> <limit> node
void ISDFParserInterface::ParseJointAxisLimit(const FXmlNode *InNode, USDFJoint *&OutJoint)
{
  // Iterate <joint> <axis> <limit> child nodes
  for (const auto &ChildNode : InNode->GetChildrenNodes())
  {
    if (ChildNode->GetTag().Equals(TEXT("lower")))
    {
      OutJoint->Axis->Lower = FCString::Atof(*ChildNode->GetContent());
    }
    else if (ChildNode->GetTag().Equals(TEXT("upper")))
    {
      OutJoint->Axis->Upper = FCString::Atof(*ChildNode->GetContent());
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
      UE_LOG(LogSDFParserInterface, Warning, TEXT("[%s][Line %d] <joint> <axis> <limit> child <%s> not supported, ignored.."),
             *FString(__FUNCTION__), __LINE__, *ChildNode->GetTag())
    }
  }
}

// From <pose>z y z r p y</pose> to FTransform
const FTransform ISDFParserInterface::PoseContentToFTransform(const FString &InPoseData)
{
  // <pose>x=[0] y=[1] z=[2] r=[3] p=[4] y=[5]</pose>
  TArray<FString> PoseDataArray;
  int32 ArrSize = InPoseData.ParseIntoArray(PoseDataArray, TEXT(" "), true);

  if (ArrSize == 6)
  {
    // roll [3], pitch [4], yaw [5] --> pitch [4], yaw [5], roll [3]
    const FRotator Rot(FMath::RadiansToDegrees(FCString::Atof(*PoseDataArray[4]) < 0 ? FCString::Atof(*PoseDataArray[4]) + 2 * PI : FCString::Atof(*PoseDataArray[4])),  // pitch
                       FMath::RadiansToDegrees(FCString::Atof(*PoseDataArray[5]) < 0 ? FCString::Atof(*PoseDataArray[5]) + 2 * PI : FCString::Atof(*PoseDataArray[5])),  // yaw
                       FMath::RadiansToDegrees(FCString::Atof(*PoseDataArray[3]) < 0 ? FCString::Atof(*PoseDataArray[3]) + 2 * PI : FCString::Atof(*PoseDataArray[3]))); // roll

    const FVector Loc(FCString::Atof(*PoseDataArray[0]),
                      FCString::Atof(*PoseDataArray[1]),
                      FCString::Atof(*PoseDataArray[2]));

    const FTransform Trans = FConversions::ROSToU(FTransform(Rot, Loc));

    return Trans;
  }
  else
  {
    UE_LOG(LogSDFParserInterface, Warning, TEXT("<pose>%s</pose> is an unsupported format!"), *InPoseData)
    return FTransform();
  }
}

// From <scale>x y z</scale> to FVector
const FVector ISDFParserInterface::ScaleContentToFVector(const FString &InScaleData)
{
  // <size>x=[0] y=[1] z=[2]</size>
  TArray<FString> DataArray;
  int32 ArrSize = InScaleData.ParseIntoArray(DataArray, TEXT(" "), true);

  if (ArrSize == 3)
  {
    return FVector(FCString::Atof(*DataArray[0]),
                   FCString::Atof(*DataArray[1]),
                   FCString::Atof(*DataArray[2]));
  }
  else
  {
    // Unsupported <pose> format, return default transform
    UE_LOG(LogSDFParserInterface, Warning, TEXT("<scale>%s</scale> is an unsupported format!"), *InScaleData)
    return FVector();
  }
}

// From <>x y z</> to FVector
const FVector ISDFParserInterface::XyzToFVector(const FString &InXyzData)
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
  else
  {
    // Unsupported <pose> format, return default transform
    UE_LOG(LogSDFParserInterface, Warning, TEXT("<xyz>%s</xyz> is an unsupported format!"), *InXyzData)
    return FVector();
  }
}

// From <size>x y z</size> to FVector
const FVector ISDFParserInterface::SizeToFVector(const FString &InSizeData)
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
  else
  {
    // Unsupported <pose> format, return default transform
    UE_LOG(LogSDFParserInterface, Warning, TEXT("<size>%s</size> is an unsupported format!"), *InSizeData)
    return FVector();
  }
}

// Add default functionality here for any ISDFParserInterface functions that are not pure virtual.
