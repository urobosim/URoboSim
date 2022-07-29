#include "Factory/RLinkFactory.h"

URLinkBuilder::URLinkBuilder()
{
  static ConstructorHelpers::FObjectFinder<UStaticMesh> LinkMeshTemp(TEXT("/URoboSim/LinkMesh.LinkMesh"));
  // static ConstructorHelpers::FObjectFinder<UStaticMesh> LinkMesh(TEXT("StaticMesh'/URoboSim/LinkMesh.LinkMesh'"));
  if (LinkMeshTemp.Succeeded())
  {
    LinkMesh = LinkMeshTemp.Object;
  }
  else
  {
      UE_LOG(LogTemp, Error, TEXT("[%s] LinkFactory: LinkMesh not found."), *FString(__FUNCTION__));
  }
}
URLink* URLinkFactory::Load(UObject* InOuter, USDFLink* InLinkDescription, FVector InLocation, FString InVersion)
{
  if(!InOuter && !InLinkDescription)
    {
      return nullptr;
    }

  LinkBuilder = CreateBuilder(InLinkDescription, InVersion);
  if(LinkBuilder)
    {
      LinkBuilder->Init(InOuter, InLinkDescription,InLocation);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("[%s] LinkFactory: LinkBuilder not created because NumCollisions = 0"), *FString(__FUNCTION__));
    }
  return LinkBuilder->NewLink();
}

URLink* URLinkFactory::Load(UObject* InOuter, USDFLink* InLinkDescription, FString InVersion)
{
  if(!InOuter && !InLinkDescription)
    {
      return nullptr;
    }

  LinkBuilder = CreateBuilder(InLinkDescription, InVersion);
  if(LinkBuilder)
    {
      LinkBuilder->Init(InOuter, InLinkDescription);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("LinkFactory: LinkBuilder not created because NumCollisions = 0"));
    }
  return LinkBuilder->NewLink();
}

URLinkBuilder* URLinkFactory::CreateBuilder(USDFLink* InLinkDescription, FString InVersion)
{
    URLinkBuilder* TempLinkBuilder = nullptr;
    TempLinkBuilder = NewObject<URLinkBuilder>(this);
    TempLinkBuilder->Version = InVersion;
    return TempLinkBuilder;
}

void URLinkBuilder::Init(UObject* InOuter, USDFLink* InLinkDescription,FVector InLocation)
{
  Model = Cast<ARModel>(InOuter);
  LinkDescription = InLinkDescription;
  LoadLocation=InLocation;
}
void URLinkBuilder::Init(UObject* InOuter, USDFLink* InLinkDescription)
{
  Model = Cast<ARModel>(InOuter);
  LinkDescription = InLinkDescription;
  LoadLocation=FVector(0,0,0);
}


URLink* URLinkBuilder::NewLink()
{
  Link = NewObject<URLink>(Model, FName((LinkDescription->Name).GetCharArray().GetData()));
  Link->CreationMethod = EComponentCreationMethod::Instance;
  Link->RegisterComponent();

  // Link->SetBoxExtent(FVector(1,1,1), false);
  Link->SetStaticMesh(LinkMesh);
  
  Link->SetSimulatePhysics(false);
  Link->SetVisibility(false);
  Link->SetMassOverrideInKg(NAME_None,0.000000);
  Link->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel18);
  Link->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
  // Link->SetCollisionProfileName("RobotNoSelfCollision");
  // Link->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  if(Model->GetRootComponent() == nullptr)
    {
      Model->SetRootComponent(Link);
    }

  if(!IsValid(Link))
  {
      UE_LOG(LogTemp, Error, TEXT("[%s] Link %s not Valid"), *FString(__FUNCTION__),  *LinkDescription->Name);
  }
  Link->PoseRelativeTo = LinkDescription->PoseRelativeTo;
  SetPose(LinkDescription->Pose);

  // Add the Collision Components to the Link
  SetCollisions();

  // Add the Visual Components to the Link
  SetVisuals();

  //Setup selfcollision
  SetCollisionProfile(LinkDescription->bSelfCollide);

  //SetInertial(Link, InLink->Inertial);

  //Enable or disable gravity for the Link
  SetSimulateGravity(LinkDescription->bGravity);

  // SetPoseComponent();

  return Link;
}

void URLinkBuilder::SetPose(FTransform InPose)
{
  Link->LinkPoseInternal = InPose;
}

void URLinkBuilder::SetVisuals()
{
  for(USDFVisual* Visual : LinkDescription->Visuals)
    {
      SetVisual(Visual);
    }
}


void URLinkBuilder::SetVisual(USDFVisual* InVisual)
{
  UStaticMeshComponent* LinkComponent = NewObject<UStaticMeshComponent>(Link, FName((InVisual->Name).GetCharArray().GetData()));
  LinkComponent->CreationMethod = EComponentCreationMethod::Instance;
  LinkComponent->RegisterComponent();
  LinkComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  // FVector LocationOffset = Link->LinkPoseInternal.GetRotation().RotateVector(InVisual->Pose.GetLocation());
  // LinkComponent->SetWorldLocation(LocationOffset + Link->LinkPoseInternal.GetLocation() + LoadLocation);

  // //Rotations are added by multiplying the Quaternions
  // FQuat RotationOffset = Link->LinkPoseInternal.GetRotation() * InVisual->Pose.GetRotation();
  // LinkComponent->SetWorldRotation(RotationOffset);

  LinkComponent->GetBodyInstance()->bAutoWeld=false;
  LinkComponent->AttachToComponent(Link, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
  LinkComponent->AddLocalTransform(InVisual->Pose);

  // LinkComponent->SetWorldLocation(InVisual->Pose.GetLocation());
  // LinkComponent->SetWorldRotation(InVisual->Pose.GetRotation());

  UStaticMesh* Visual = InVisual->Geometry->Mesh;
  if(Visual)
    {
      LinkComponent->SetStaticMesh(Visual);
      int32 Index = InVisual->Name.Find(TEXT("visual"),ESearchCase::CaseSensitive,ESearchDir::FromStart,0);
      UStaticMeshComponent* MatchCollision = Link->GetCollision(InVisual->Name.Left(Index),false);

      if(MatchCollision)
        {
          LinkComponent->AttachToComponent(MatchCollision, FAttachmentTransformRules::KeepWorldTransform);
        }
      else if(Link->Visuals.Num() > 0)
        {
          LinkComponent->AttachToComponent(Link->Visuals[0], FAttachmentTransformRules::KeepWorldTransform);
        }
      Link->Visuals.Add(LinkComponent);
    }
}


void URLinkBuilder::SetCollisions()
{
    for(USDFCollision* Collision : LinkDescription->Collisions)
    {
        SetCollision(Collision);
    }
}



void URLinkBuilder::SetCollision(USDFCollision* InCollision)
{
  UStaticMeshComponent* LinkComponent = NewObject<UStaticMeshComponent>(Link, FName((InCollision->Name).GetCharArray().GetData()));
  LinkComponent->CreationMethod = EComponentCreationMethod::Instance;
  LinkComponent->RegisterComponent();

  LinkComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
  LinkComponent->BodyInstance.PositionSolverIterationCount = 50;
  LinkComponent->BodyInstance.VelocitySolverIterationCount = 50;

  Link->Collisions.Add(LinkComponent);

  // FVector LocationOffset = Link->LinkPoseInternal.GetRotation().RotateVector(InCollision->Pose.GetLocation());
  // FVector FinalPos = LocationOffset + Link->LinkPoseInternal.GetLocation() + LoadLocation;
  // LinkComponent->SetWorldLocation(FinalPos);

  LinkComponent->AttachToComponent(Link, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
  LinkComponent->AddLocalTransform(InCollision->Pose);
  LinkComponent->WeldTo(Link);


  //Rotations are added by multiplying the Quaternions
  // FQuat RotationOffset = Link->LinkPoseInternal.GetRotation() * InCollision->Pose.GetRotation();
  // LinkComponent->SetWorldRotation(InCollision->Pose.GetRotation());

  //Static Mesh creation
  UStaticMesh* Collision = InCollision->Geometry->Mesh;
  if(Collision)
    {
      LinkComponent->SetStaticMesh(Collision);
      LinkComponent->SetVisibility(false,false);
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Collision Mesh for %s not valid"), *LinkComponent->GetName());
    }
}

void URLinkBuilder::SetInertial(USDFLinkInertial* InInertial)
{
  for(UStaticMeshComponent* Visual : Link->Visuals)
    {
      Visual->SetMassOverrideInKg(NAME_None, 0.001, true);
    }
  for(UStaticMeshComponent* Collision : Link->Collisions)
    {
      Collision->SetMassOverrideInKg(NAME_None, 0.001, true);
    }
  if(Link)
    {
      Link->SetMassOverrideInKg(NAME_None, InInertial->Mass, true);
    }
}

void URLinkBuilder::SetCollisionProfile(bool InSelfColide)
{
  for(UStaticMeshComponent* Visual : Link->Visuals)
    {
      // Visual->SetCollisionProfileName("NoCollision");
      //Visual->SetCollisionProfileName("RobotVisualNoCollision");
      Visual->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel18);
      Visual->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    }

  for(UStaticMeshComponent* Collision : Link->Collisions)
    {
      Collision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel18);
      Collision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
      if(Link->Visuals.Num() == 0)
        {
          Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
        }
      else
        {
          Collision->SetCollisionProfileName("RobotWithSelfCollision");
          // Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
        }
      if(!InSelfColide)
        {
          Collision->SetCollisionProfileName("RobotNoSelfCollision");
          // Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel18, ECollisionResponse::ECR_Ignore);
        }
    }
}

void URLinkBuilder::SetSimulateGravity(bool InUseGravity)
{
  for(UStaticMeshComponent* Collision : Link->Collisions)
    {
      Collision->SetEnableGravity(false);
    }
  if(Link)
    {
      Link->SetEnableGravity(InUseGravity);
    }

  for(UStaticMeshComponent* Visual : Link->Visuals)
    {
      Visual->SetEnableGravity(false);
    }
}

void URLinkBuilder::SetPoseComponent()
{
  // USceneComponent *PoseComponent = NewObject<USceneComponent>(Link, *(Link->GetName() + TEXT("Pose")));
  // PoseComponent->AttachToComponent(Link->Collisions[0], FAttachmentTransformRules::KeepWorldTransform);
  // // PoseComponent->SetWorldLocation(LoadLocation);
  // // PoseComponent->AddWorldTransform(Link->LinkPoseInternal);
  // PoseComponent->SetWorldTransform(Link->LinkPoseInternal);
  // Link->SetPoseComponent(PoseComponent);
}
