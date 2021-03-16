#include "Factory/RLinkBuilder.h"

DEFINE_LOG_CATEGORY_STATIC(LogRLinkBuilder, Log, All);

bool URLinkBuilder::LoadSDF(USDFLink *&SDFLink)
{
  if (!Model || !SDFLink)
  {
    return false;
  }
  else
  {
    if (SDFLink->Collisions.Num() > 0)
    {
      SetNewLink(SDFLink);
      return true;
    }
    else
    {
      UE_LOG(LogRLinkBuilder, Error, TEXT("Link %s not created because NumCollisions = 0"), *SDFLink->GetName());
      return false;
    }
  }
}

void URLinkBuilder::SetNewLink(USDFLink *&SDFLink)
{
  Link = NewObject<URLink>(Model, *SDFLink->GetName());

  // Add CollisionMeshes to the link
  SetCollisionMeshes(SDFLink);

  // Add VisualMeshes to the link
  SetVisualMeshes(SDFLink);

  // Setup self-collision
  SetCollisionProfile(SDFLink->bSelfCollide);

  // Set inertial
  SetInertial(SDFLink->Inertial);

  // Set root pose
  SetPose(SDFLink->Pose);

  Model->AddLink(Link);
}

void URLinkBuilder::SetVisualMeshes(USDFLink *&SDFLink)
{
  for (USDFVisual *&SDFVisual : SDFLink->Visuals)
  {
    UStaticMeshComponent *LinkMeshComponent = NewObject<UStaticMeshComponent>(Link, *SDFVisual->GetName());
    LinkMeshComponent->RegisterComponent();

    FVector LocationOffset = SDFLink->Pose.GetRotation().RotateVector(SDFVisual->Pose.GetLocation());
    LinkMeshComponent->SetWorldLocation(SDFLink->Pose.GetLocation() + LocationOffset);

    // Rotations are added by multiplying the rotations
    FQuat RotationOffset = SDFLink->Pose.GetRotation() * SDFVisual->Pose.GetRotation();
    LinkMeshComponent->SetWorldRotation(RotationOffset);

    if (SDFVisual && SDFVisual->Geometry && SDFVisual->Geometry->Mesh)
    {
      LinkMeshComponent->SetStaticMesh(SDFVisual->Geometry->Mesh);
      LinkMeshComponent->SetSimulatePhysics(false);
      LinkMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
      LinkMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
      LinkMeshComponent->bVisible = true;
      LinkMeshComponent->WeldTo(Link->GetCollisionMeshes()[0]);
      Link->AddVisualMesh(LinkMeshComponent);
    }
    else
    {
      UE_LOG(LogRLinkBuilder, Error, TEXT("VisualMesh not valid"));
    }
  }
}

void URLinkBuilder::SetCollisionMeshes(USDFLink *&SDFLink)
{
  for (USDFCollision *&SDFCollision : SDFLink->Collisions)
  {
    UStaticMeshComponent *LinkMeshComponent = NewObject<UStaticMeshComponent>(Link, *SDFCollision->GetName());
    LinkMeshComponent->RegisterComponent();
    if (Model->GetRootComponent() == nullptr)
    {
      Model->SetRootComponent(LinkMeshComponent);
    }
    LinkMeshComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
    LinkMeshComponent->BodyInstance.PositionSolverIterationCount = 20;
    LinkMeshComponent->BodyInstance.VelocitySolverIterationCount = 8;

    FVector LocationOffset = SDFLink->Pose.GetRotation().RotateVector(SDFCollision->Pose.GetLocation());
    LinkMeshComponent->SetWorldLocation(LocationOffset + SDFLink->Pose.GetLocation());

    // Rotations are added by multiplying the Quaternions
    FQuat RotationOffset = SDFLink->Pose.GetRotation() * SDFCollision->Pose.GetRotation();
    LinkMeshComponent->SetWorldRotation(RotationOffset);

    // Static Mesh creation
    if (SDFCollision && SDFCollision->Geometry && SDFCollision->Geometry->Mesh)
    {
      LinkMeshComponent->SetStaticMesh(SDFCollision->Geometry->Mesh);
      LinkMeshComponent->bVisible = false;
      if (Link->GetCollisionMeshes().Num() == 0)
      {
        LinkMeshComponent->SetSimulatePhysics(true);
        LinkMeshComponent->AttachToComponent(Model->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
      }
      else
      {
        LinkMeshComponent->SetSimulatePhysics(false);
        LinkMeshComponent->WeldTo(Link->GetCollisionMeshes()[0]);
      }
      Link->AddCollisionMesh(LinkMeshComponent);
    }
    else
    {
      UE_LOG(LogRLinkBuilder, Error, TEXT("CollisionMesh not valid"));
    }
  }
}

void URLinkBuilder::SetInertial(USDFLinkInertial *&InInertial)
{
  if (Link->GetCollisionMeshes().Num() > 0)
  {
    Link->GetCollisionMeshes()[0]->SetMassOverrideInKg(NAME_None, InInertial->Mass, true);
  }
}

void URLinkBuilder::SetCollisionProfile(const bool &bSelfColide)
{
  for (UStaticMeshComponent *CollisionMesh : Link->GetCollisionMeshes())
  {
    CollisionMesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
    CollisionMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
    if (Link->GetVisualMeshes().Num() == 0)
    {
      CollisionMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    }
    else
    {
      CollisionMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    }
    if (!bSelfColide)
    {
      CollisionMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
    }
  }
}

void URLinkBuilder::SetPose(const FTransform &Pose)
{
  USceneComponent *PoseComponent = NewObject<USceneComponent>(Link, *Link->GetName());
  PoseComponent->AttachToComponent(Link->GetCollisionMeshes()[0], FAttachmentTransformRules::KeepRelativeTransform);
  PoseComponent->SetWorldTransform(Pose);
  Link->SetPoseComponent(PoseComponent);
}
