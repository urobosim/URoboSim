#include "RStaticMeshEditUtils.h"
#include "Factory/RLinkFactory.h"

URLink* URLinkFactory::Load(UObject* InOuter, USDFLink* InLinkDescription)
{
  if(!InOuter && !InLinkDescription)
    {
      return nullptr;
    }

  LinkBuilder = CreateBuilder(InLinkDescription);
  LinkBuilder->Init(InOuter, InLinkDescription);

  return LinkBuilder->NewLink();
}

URLinkBuilder* URLinkFactory::CreateBuilder(USDFLink* InLinkDescription)
{
  if(InLinkDescription->Collisions.Num()>0)
    {
      return NewObject<URLinkBuilder>(this);
    }
  else
    {
      return NewObject<URVirtualLinkBuilder>(this);
    }
}

void URLinkBuilder::Init(UObject* InOuter, USDFLink* InLinkDescription)
{
  Model = Cast<ARModel>(InOuter);
  LinkDescription = InLinkDescription;
}

URLink* URLinkBuilder::NewLink()
{
  Link = NewObject<URLink>(Model, FName((LinkDescription->Name).GetCharArray().GetData()));
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

  return Link;
}

void URLinkBuilder::SetPose(FTransform InPose)
{
  LinkPose = InPose;
}

void URLinkBuilder::SetPose(FVector InLocation, FQuat InRotation)
{
  LinkPose.SetLocation(InLocation);
  LinkPose.SetRotation(InRotation);
}

bool URLinkBuilder::CreateCollisionForMesh(UStaticMesh* OutMesh, ESDFGeometryType Type)
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
      UE_LOG(LogTemp, Error, TEXT("GeometryType not supportet."));
      return false;
    }
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
  URStaticMeshComponent* LinkComponent = NewObject<URStaticMeshComponent>(Link, FName((InVisual->Name).GetCharArray().GetData()));

  FVector LocationOffset = LinkPose.GetRotation().RotateVector(InVisual->Pose.GetLocation());
  LinkComponent->SetWorldLocation(LocationOffset + LinkPose.GetLocation());

  //Rotations are added by multiplying the Quaternions
  FQuat RotationOffset = LinkPose.GetRotation() * InVisual->Pose.GetRotation();
  LinkComponent->SetWorldRotation(RotationOffset);

  //Static Mesh creation
  UStaticMesh* Visual = RStaticMeshUtils::CreateStaticMesh(LinkComponent, InVisual);
  if(Visual)
    {
      //Create the collision vor the visual mesh. Necessary to enable physics.
      CreateCollisionForMesh(Visual, InVisual->Geometry->Type);
      LinkComponent->SetStaticMesh(Visual);

      if(Link->Visuals.Num()==0)
        {
          if(Link->Collisions.Num()!=0)
            {
              // LinkComponent->WeldTo(Link->Collisions[0]);
              LinkComponent->GetBodyInstance()->bAutoWeld=false;
              LinkComponent->AttachToComponent(Link->Collisions[0], FAttachmentTransformRules::KeepWorldTransform);
            }
        }
      else
        {
          LinkComponent->WeldTo(Link->Visuals[0]);
          UE_LOG(LogTemp, Warning, TEXT("Attachment succesfull Weld to first visual:%s."), *LinkComponent->GetName() );
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
  URStaticMeshComponent* LinkComponent = NewObject<URStaticMeshComponent>(Link, FName((InCollision->Name).GetCharArray().GetData()));
  if(Model->GetRootComponent() == nullptr)
    {
      Model->SetRootComponent(LinkComponent);
    }

  LinkComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
  LinkComponent->BodyInstance.PositionSolverIterationCount = 20;
  LinkComponent->BodyInstance.VelocitySolverIterationCount = 8;

  FVector LocationOffset = LinkPose.GetRotation().RotateVector(InCollision->Pose.GetLocation());
  LinkComponent->SetWorldLocation(LocationOffset + LinkPose.GetLocation());

  //Rotations are added by multiplying the Quaternions
  FQuat RotationOffset = LinkPose.GetRotation() * InCollision->Pose.GetRotation();
  LinkComponent->SetWorldRotation(RotationOffset);

  //Static Mesh creation
  UStaticMesh* Collision = RStaticMeshUtils::CreateStaticMesh(LinkComponent, InCollision);
  if(Collision)
    {
      //Create the collision vor the visual mesh. Necessary to enable physics.
      CreateCollisionForMesh(Collision, InCollision->Geometry->Type);
      LinkComponent->SetStaticMesh(Collision);

      if(Link->Collisions.Num()==0)
        {
          LinkComponent->SetSimulatePhysics(true);
          LinkComponent->AttachToComponent(Model->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
        }
      else
        {
          LinkComponent->AttachToComponent(Link->Collisions[0], FAttachmentTransformRules::KeepWorldTransform);
          LinkComponent->WeldTo(Link->Collisions[0]);
        }
      LinkComponent->bVisible = false;
      Link->Collisions.Add(LinkComponent);
    }
}

void URLinkBuilder::SetInertial(USDFLinkInertial* InInertial)
{
  for(URStaticMeshComponent* Visual : Link->Visuals)
    {
      Visual->SetMassOverrideInKg(NAME_None, 0.001, true);
    }
  for(URStaticMeshComponent* Collision : Link->Collisions)
    {
      Collision->SetMassOverrideInKg(NAME_None, 0.001, true);
    }
  if(Link->GetCollision())
    {
      Link->GetCollision()->SetMassOverrideInKg(NAME_None, InInertial->Mass, true);
    }
}

void URLinkBuilder::SetCollisionProfile(bool InSelfColide)
{
  for(URStaticMeshComponent* Visual : Link->Visuals)
    {
      Visual->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
      Visual->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    }

  for(URStaticMeshComponent* Collision : Link->Collisions)
    {
      Collision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
      Collision->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
      Collision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
      if(!InSelfColide)
        {
          Collision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
        }
    }
}

void URLinkBuilder::SetSimulateGravity(bool InUseGravity)
{
  for(URStaticMeshComponent* Collision : Link->Collisions)
    {
      Collision->SetEnableGravity(false);
    }
  if(Link->GetCollision())
    {
      Link->GetCollision()->SetEnableGravity(InUseGravity);
    }

  for(URStaticMeshComponent* Visual : Link->Visuals)
    {
      Visual->SetEnableGravity(false);
    }
}

void URVirtualLinkBuilder::SetCollisions()
{
  URStaticMeshComponent* LinkComponent = NewObject<URStaticMeshComponent>(Link, FName((Link->GetName()).GetCharArray().GetData()));
  if(Model->GetRootComponent() == nullptr)
    {
      Model->SetRootComponent(LinkComponent);
    }

  LinkComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
  LinkComponent->BodyInstance.PositionSolverIterationCount = 20;
  LinkComponent->BodyInstance.VelocitySolverIterationCount = 8;

  LinkComponent->SetWorldLocation(LinkPose.GetLocation());

  //Rotations are added by multiplying the Quaternions
  // FQuat RotationOffset = Link->Pose.GetRotation() * InLink->Pose.GetRotation();
  FQuat RotationOffset = LinkPose.GetRotation();
  LinkComponent->SetWorldRotation(RotationOffset);

  USDFCollision* VirtualCollision = NewObject<USDFCollision>(Link);
  VirtualCollision->Name = Link->GetName();
  VirtualCollision->Name = VirtualCollision->Name.Append("_geom");
  VirtualCollision->Pose = FTransform();
  VirtualCollision->Geometry = NewObject<USDFGeometry>(Link);
  VirtualCollision->Geometry->Type = ESDFGeometryType::Box;
  VirtualCollision->Geometry->Size = FVector(0.5f, 0.5f, 0.5f);

  //Static Mesh creation
  UStaticMesh* Collision = RStaticMeshUtils::CreateStaticMesh(LinkComponent, VirtualCollision);
  if(Collision)
    {
      //Create the collision vor the visual mesh. Necessary to enable physics.
      CreateCollisionForMesh(Collision, VirtualCollision->Geometry->Type);
      LinkComponent->SetStaticMesh(Collision);
      /*  */
      /* if(Link->Collisions.Num()==0) */
      /* { */
      /* 	UE_LOG(LogTemp, Error, TEXT("before AttachToComponent")); */
      LinkComponent->SetSimulatePhysics(true);
      LinkComponent->AttachToComponent(Model->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
      /* 	UE_LOG(LogTemp, Error, TEXT("after AttachToComponent")); */
      /* } */
      LinkComponent->bVisible = false;
      Link->Collisions.Add(LinkComponent);
    }
}

void URVirtualLinkBuilder::SetVisuals()
{
}
