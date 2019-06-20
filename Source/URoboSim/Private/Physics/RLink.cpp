// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "Physics/RLink.h"
#include "Physics/RJoint.h"
#include "Physics/RModel.h"
#include "RStaticMeshEditUtils.h"


// Sets default values for this component's properties
URLink::URLink()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// PrimaryComponentTick.bCanEverTick = true;
	// PrimaryComponentTick.TickGroup = TG_PrePhysics;

	// ...
}

void URLink::SetPose(FTransform InPose)
{
	Pose = InPose;

}

void URLink::SetPose(FVector InLocation, FQuat InRotation)
{
	Pose.SetLocation(InLocation);
	Pose.SetRotation(InRotation);

}

void URLink::AddJoint(URJoint* InJoint)
{
	Joints.Add(InJoint);
}

// Load link from sdf data
void URLink::Load(ARModel* OutModel, USDFLink* InLink)
{
	//TODO only create Link if collision exists

	if(InLink->Collisions.Num()>0)
	{
		URLink* Link = NewObject<URLink>(OutModel, FName((InLink->Name).GetCharArray().GetData()));
		Link->Model = OutModel;

		Link->SetPose(InLink->Pose);

		//Add the Collision Components to the Link
		SetCollisions(Link, InLink->Collisions);

		// Add the Visual Components to the Link
		SetVisuals(Link, InLink->Visuals);

		//Setup selfcollision
		SetCollisionProfile(Link, InLink->bSelfCollide);

		//SetInertial(Link, InLink->Inertial);

		//Enable or disable gravity for the Link
		SetSimulateGravity(Link, InLink->bGravity);
		//Add the Link to the list of links of the model
		OutModel->AddLink(Link);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Collision in %s. Create Virtual Link."), *InLink->Name);
		URLink* Link = NewObject<URLink>(OutModel, FName((InLink->Name).GetCharArray().GetData()));
		Link->Model = OutModel;

		Link->SetPose(InLink->Pose);
		SetVirtualCollision(Link, InLink);

		if(Link->GetCollision())
		{
			Link->GetCollision()->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
			Link->GetCollision()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		}

		//Enable or disable gravity for the Link
		SetSimulateGravity(Link, false);

		OutModel->AddLink(Link);
	}
}

void URLink::SetVirtualCollision(URLink* OutLink, USDFLink* InLink)
{
    URStaticMeshComponent* LinkComponent = NewObject<URStaticMeshComponent>(OutLink, FName((InLink->Name).GetCharArray().GetData()));
	if(OutLink->Model->GetRootComponent() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Root set"));
		OutLink->Model->SetRootComponent(LinkComponent);
	}

	LinkComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	LinkComponent->BodyInstance.PositionSolverIterationCount = 20;
	LinkComponent->BodyInstance.VelocitySolverIterationCount = 8;

	/* FVector LocationOffset = OutLink->Pose.GetRotation().RotateVector(InLink->Pose.GetLocation()); */
	/* UE_LOG(LogTemp, Warning, TEXT("LocationOffset %s"), *LocationOffset.ToString()); */
    // LinkComponent->SetWorldLocation(LocationOffset + OutLink->Pose.GetLocation());
    LinkComponent->SetWorldLocation(OutLink->Pose.GetLocation());

	//Rotations are added by multiplying the Quaternions
	// FQuat RotationOffset = OutLink->Pose.GetRotation() * InLink->Pose.GetRotation();
	FQuat RotationOffset = OutLink->Pose.GetRotation();
    LinkComponent->SetWorldRotation(RotationOffset);

	USDFCollision* VirtualCollision = NewObject<USDFCollision>(InLink);
	VirtualCollision->Name = InLink->Name;
	VirtualCollision->Name = VirtualCollision->Name.Append("_geom");
	VirtualCollision->Pose = FTransform();
	VirtualCollision->Geometry = NewObject<USDFGeometry>(InLink);
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
        /* if(OutLink->Collisions.Num()==0) */
        /* { */
		/* 	UE_LOG(LogTemp, Error, TEXT("before AttachToComponent")); */
		LinkComponent->SetSimulatePhysics(true);
		LinkComponent->AttachToComponent(OutLink->Model->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		/* 	UE_LOG(LogTemp, Error, TEXT("after AttachToComponent")); */
		/* } */
		LinkComponent->bVisible = false;
		OutLink->Collisions.Add(LinkComponent);
	}
}

void URLink::SetVisuals(URLink* OutLink, TArray<USDFVisual*> InVisuals)
{
    for(USDFVisual* Visual : InVisuals)
    {
        SetVisual(OutLink, Visual);
    }
}

void URLink::SetVisual(URLink* OutLink, USDFVisual* InVisual)
{

    URStaticMeshComponent* LinkComponent = NewObject<URStaticMeshComponent>(OutLink, FName((InVisual->Name).GetCharArray().GetData()));
	LinkComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;

	FVector LocationOffset = OutLink->Pose.GetRotation().RotateVector(InVisual->Pose.GetLocation());
    LinkComponent->SetWorldLocation(LocationOffset + OutLink->Pose.GetLocation());

	//Rotations are added by multiplying the Quaternions
	FQuat RotationOffset = OutLink->Pose.GetRotation() * InVisual->Pose.GetRotation();
    LinkComponent->SetWorldRotation(RotationOffset);

    // LinkComponent->SetWorldRotation(InVisual->Pose.GetRotation() * OutLink->Pose.GetRotation());
    // LinkComponent->SetWorldRotation(InVisual->Pose.GetRotation() * OutLink->Pose.GetRotation());

    //Static Mesh creation
    UStaticMesh* Visual = RStaticMeshUtils::CreateStaticMesh(LinkComponent, InVisual);
    if(Visual)
    {
        //Create the collision vor the visual mesh. Necessary to enable physics.
        CreateCollisionForMesh(Visual, InVisual->Geometry->Type);
        LinkComponent->SetStaticMesh(Visual);

        if(OutLink->Visuals.Num()==0)
        {
			if(OutLink->Collisions.Num()!=0)
			{
				// LinkComponent->WeldTo(OutLink->Collisions[0]);
				LinkComponent->GetBodyInstance()->bAutoWeld=false;
				// LinkComponent->SetWorldRotation(OutLink->Collisions[0]->GetComponentRotation());

				LinkComponent->AttachToComponent(OutLink->Collisions[0], FAttachmentTransformRules::KeepWorldTransform);

                UE_LOG(LogTemp, Warning, TEXT("Attachment succesfull Weld:%s."), *LinkComponent->GetName() );
				// if()
				// {
				// 	UE_LOG(LogTemp, Log, TEXT("Attachment succesfull Weld:%s."), LinkComponent->GetBodyInstance()->bAutoWeld ? TEXT("true") : TEXT("false"));
				// }
			}
        }
        else
        {
			// LinkComponent->SetWorldRotation(OutLink->Collisions[0]->Pose.GetRotation());
			// LinkComponent->AttachToComponent(OutLink->Visuals[0], FAttachmentTransformRules::KeepWorldTransform);
            LinkComponent->WeldTo(OutLink->Visuals[0]);
            UE_LOG(LogTemp, Warning, TEXT("Attachment succesfull Weld to first visual:%s."), *LinkComponent->GetName() );
        }
		OutLink->Visuals.Add(LinkComponent);
    }
}

void URLink::SetCollisions(URLink* OutLink, TArray<USDFCollision*> InCollisions)
{
    for(USDFCollision* Collision : InCollisions)
    {
        SetCollision(OutLink, Collision);
    }
}


void URLink::SetCollision(URLink* OutLink, USDFCollision* InCollision)
{
    URStaticMeshComponent* LinkComponent = NewObject<URStaticMeshComponent>(OutLink, FName((InCollision->Name).GetCharArray().GetData()));
	if(OutLink->Model->GetRootComponent() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Root set"));
		OutLink->Model->SetRootComponent(LinkComponent);
	}

	LinkComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
	LinkComponent->BodyInstance.PositionSolverIterationCount = 20;
	LinkComponent->BodyInstance.VelocitySolverIterationCount = 8;

	FVector LocationOffset = OutLink->Pose.GetRotation().RotateVector(InCollision->Pose.GetLocation());
    LinkComponent->SetWorldLocation(LocationOffset + OutLink->Pose.GetLocation());

	//Rotations are added by multiplying the Quaternions
	FQuat RotationOffset = OutLink->Pose.GetRotation() * InCollision->Pose.GetRotation();
    LinkComponent->SetWorldRotation(RotationOffset);

    // LinkComponent->SetWorldLocation(InCollision->Pose.GetLocation());
    // LinkComponent->SetWorldRotation(InCollision->Pose.GetRotation());

    //Static Mesh creation
    UStaticMesh* Collision = RStaticMeshUtils::CreateStaticMesh(LinkComponent, InCollision);
    if(Collision)
    {
        //Create the collision vor the visual mesh. Necessary to enable physics.
        CreateCollisionForMesh(Collision, InCollision->Geometry->Type);
        LinkComponent->SetStaticMesh(Collision);

        if(OutLink->Collisions.Num()==0)
        {
            LinkComponent->SetSimulatePhysics(true);
			LinkComponent->AttachToComponent(OutLink->Model->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
        }
        else
        {
			LinkComponent->AttachToComponent(OutLink->Collisions[0], FAttachmentTransformRules::KeepWorldTransform);
            LinkComponent->WeldTo(OutLink->Collisions[0]);
        }
		LinkComponent->bVisible = false;
		OutLink->Collisions.Add(LinkComponent);
		// CleanUp(InCollision->Name);
	}
}

void URLink::SetInertial(URLink* OutLink, USDFLinkInertial* InInertial)
{
	for(URStaticMeshComponent* Visual : OutLink->Visuals)
	{
		Visual->SetMassOverrideInKg(NAME_None, 0.001, true);
	}
	for(URStaticMeshComponent* Collision : OutLink->Collisions)
	{
		Collision->SetMassOverrideInKg(NAME_None, 0.001, true);
	}
	if(OutLink->GetCollision())
	{
		OutLink->GetCollision()->SetMassOverrideInKg(NAME_None, InInertial->Mass, true);
	}

}

void URLink::SetCollisionProfile(URLink* OutLink, bool InSelfColide)
{
    for(URStaticMeshComponent* Visual : OutLink->Visuals)
    {
        Visual->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
        Visual->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    }

    for(URStaticMeshComponent* Collision : OutLink->Collisions)
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

void URLink::SetSimulateGravity(URLink* OutLink, bool InUseGravity)
{
    for(URStaticMeshComponent* Collision : OutLink->Collisions)
    {
        Collision->SetEnableGravity(false);
    }
	if(OutLink->GetCollision())
	{
		OutLink->GetCollision()->SetEnableGravity(InUseGravity);
	}

    for(URStaticMeshComponent* Visual : OutLink->Visuals)
    {
        Visual->SetEnableGravity(false);
    }
}


bool URLink::CreateCollisionForMesh(UStaticMesh* OutMesh, ESDFGeometryType Type)
{
    // TArray<float> Parameters = GetGeometryParameter(InVisual.Geometry);
    switch(Type)
    {
        case ESDFGeometryType::None :
            // UE_LOG(LogTemp, Log, TEXT("GeometryTyp Not Implemented"));
            return false;
        case ESDFGeometryType::Mesh :
            // RStaticMeshUtils::CreateComplexCollision(OutMesh, 1.0, 32);
            return true;
        case ESDFGeometryType::Box :
            RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopX10);
            return true;
        case ESDFGeometryType::Cylinder :
            RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopZ10);
            // RStaticMeshUtils::CreateComplexCollision(OutMesh, 4, 16, 100000);
            return true;
        case ESDFGeometryType::Sphere :
            RStaticMeshUtils::GenerateKDop(OutMesh, ECollisionType::DopX10);
            // RStaticMeshUtils::CreateComplexCollision(OutMesh, 4, 16, 100000);
            return true;
        default :
            UE_LOG(LogTemp, Error, TEXT("GeometryType not supportet."));
            return false;
    }

}


URStaticMeshComponent* URLink::GetVisual()
{
	if(Visuals.Num()>0)
	{
		return Visuals[0];
	}
	else
	{
		return nullptr;
	}
}

URStaticMeshComponent* URLink::GetCollision()
{
	if(Collisions.Num()>0)
	{
		return Collisions[0];
	}
	else
	{
		return nullptr;
	}
}

FString URLink::GetParentFrame(FString InDefaultFrame)
{
	if(ParentFrame.IsEmpty())
	{
		return InDefaultFrame;
	}
	return ParentFrame;
}

float URLink::GetNumCollisions()
{
	return Collisions.Num();
}

// void URLink::UpdateJointStates()
// {
//   for(URJoint* Joint : Joints)
//     {
//       Joint->UpdateJointStates();
//     }
// }

void URLink::UpdateVelocity()
{
  for(URJoint* Joint : Joints)
    {
      Joint->UpdateVelocity();
    }
}

void URLink::UpdateEncoder()
{
  for(URJoint* Joint : Joints)
    {
      Joint->UpdateEncoder();
    }
}

void URLink::SetNextVelocities()
{
	if(Collisions.Num()>0)
	{
		Collisions[0]->SetNextVelocities();
	}
}
