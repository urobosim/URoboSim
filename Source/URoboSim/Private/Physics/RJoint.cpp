// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#include "Physics/RJoint.h"
#include "Physics/RLink.h"
#include "Physics/RModel.h"
#include "RStaticMeshComponent.h"


// Sets default values for this component's properties
URJoint::URJoint()
{
  bBreakEnabled = false;
  bActuate = true;
}


float URJoint::GetEncoderValue()
{
  return Constraint->Encoder->GetValue();
}

void URJoint::UpdateEncoder()
{
  Constraint->UpdateEncoderValue(GetJointPosition());
  Child->UpdateEncoder();
}

void URJoint::EnableMotor(bool InEnable)
{
  Constraint->EnableMotor(InEnable);
}

void URJoint::UpdateVelocity()
{
  if(bActuate)
  {
    Constraint->UpdateJointVelocity();
  }

  Child->UpdateVelocity();
}

// void URJoint::UpdateJointStates()
// {
// 	Constraint->UpdateJointVelocity();
// 	Child_->UpdateJointStates();
// }

void URJoint::Load(ARModel* OutModel, USDFJoint* InJoint)
{
  UActorComponent* ParentMesh = nullptr;
  UActorComponent* ChildMesh = nullptr;
  URLink* ParentLink;
  URLink* ChildLink;

  for(auto& Component : OutModel->Links)
    {
      if(Component.Key.Equals(InJoint->Parent))
        {
          ParentMesh = Component.Value->GetCollision();
          ParentLink = Component.Value;
        }
      if(Component.Key.Equals(InJoint->Child))
        {
          ChildMesh = Component.Value->GetCollision();
          ChildLink = Component.Value;
        }
    }

  if(ParentMesh && ChildMesh)
    {
      URJoint* Joint = NewObject<URJoint>(OutModel, FName((InJoint->Name).GetCharArray().GetData()));
      if(CreateConstraintComponent(Joint, InJoint))
        {
          ChildLink->ParentFrame = ParentLink->GetName();
          Joint->Child = ChildLink;
          Joint->Parent = ParentLink;
          Joint->Pose = InJoint->Pose;
          InitConstraintComponent(Joint,InJoint);
          ParentLink->AddJoint(Joint);
          OutModel->AddJoint(Joint);
        }
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("Parent and/or Child not found for Link: %s"), *InJoint->Name);
    }
}

void URJoint::InitConstraintComponent(URJoint* OutJoint, USDFJoint* InJoint)
{

	OutJoint->Constraint->SetParentChild(Cast<URStaticMeshComponent>(OutJoint->Parent->GetCollision()), Cast<URStaticMeshComponent>(OutJoint->Child->GetCollision()));
	// OutJoint->Constraint->SetPosition(InJoint);
	SetConstraintPosition(OutJoint, InJoint);
	OutJoint->Constraint->SetAxis(InJoint);
	OutJoint->Constraint->ConnectToComponents();
	OutJoint->Constraint->Upper = InJoint->Axis->Upper;
	OutJoint->Constraint->Lower = InJoint->Axis->Lower;
}

void URJoint::SetConstraintPosition(URJoint* OutJoint, USDFJoint* InJoint)
{
  URStaticMeshComponent* Child = Cast<URStaticMeshComponent>(OutJoint->Child->GetCollision());
  URStaticMeshComponent* Parent = Cast<URStaticMeshComponent>(OutJoint->Parent->GetCollision());
  URConstraintComponent* Constraint = OutJoint->Constraint;
  FQuat ChildRotation = Child->GetComponentQuat();
  FQuat ParentRotation = Parent->GetComponentQuat();

  if(InJoint->Axis->bUseParentModelFrame)
    {
      FVector ModelLocation = Cast<ARModel>(OutJoint->GetOuter())->GetActorLocation();
      FQuat ModelRotation = Cast<ARModel>(OutJoint->GetOuter())->GetActorQuat();

      //TODO test order of multiplication
      FVector LocationOffset = ModelRotation.RotateVector(InJoint->Pose.GetLocation());
      Constraint->SetWorldLocation(Child->GetComponentLocation() + LocationOffset);

      //TODO test order of multiplication
      FQuat RotationOffset = ChildRotation * InJoint->Pose.GetRotation();
      Constraint->SetWorldRotation(RotationOffset);

      Constraint->AttachToComponent(Child, FAttachmentTransformRules::KeepWorldTransform);
    }
  else
    {
      //TODO Implement and check this case
      UE_LOG(LogTemp, Warning, TEXT("Does't use parent frame"));

      Constraint->SetPosition(InJoint);
      Constraint->AttachToComponent(Child, FAttachmentTransformRules::KeepRelativeTransform);
    }

}

bool URJoint::CreateConstraintComponent(URJoint* OutOwner, USDFJoint* InJoint)
{

	FString Name = FString(*InJoint->Name) + TEXT("_constraint");
    if(InJoint->Type.Equals("revolute"))
    {
		if((InJoint->Axis->Upper > 360) ||
		   (InJoint->Axis->Lower < -360))
		{
			OutOwner->Constraint = NewObject<URContinuousConstraintComponent>(OutOwner, FName(*Name));
		}
		else
		{
			OutOwner->Constraint = NewObject<URRevoluteConstraintComponent>(OutOwner, FName(*Name));
		}
		return true;
    }
	else if(InJoint->Type.Equals("prismatic"))
	{
        OutOwner->Constraint = NewObject<URPrismaticConstraintComponent>(OutOwner, FName(*Name));
		return true;
	}
	// else if(InJoint->Type.Equals("screw"))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("Create Screw"));
    //     OutOwner->Constraint = NewObject<URScrewConstraintComponent>(OutOwner, FName(*Name));
	// 	return true;
	// }
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Constraint Type not supported."), *InJoint->Type);
		return false;
	}
}

void URJoint::SetParentChild(URLink* InParent, URLink* InChild)
{
  Child = InChild;
  Parent = InParent;

  Constraint->SetParentChild(Parent->GetCollision(), Child->GetCollision());
}

// URLink* URJoint::Child()
// {
// 	return Child_;
// }

// URLink* URJoint::Parent()
// {
// 	return Parent_;
// }

float URJoint::GetJointPosition()
{
	return Constraint->GetJointPosition();

}

float URJoint::GetJointPositionInUUnits()
{
	return Constraint->GetJointPositionInUUnits();

}

float URJoint::GetJointVelocity()
{
	return Constraint->GetJointVelocity();
}

void URJoint::SetJointPosition(float Angle)
{
	Constraint->SetJointPosition(Angle);
}
void URJoint::SetJointVelocity(float Velocity)
{
	Constraint->SetJointVelocity(Velocity);
}

void URJoint::SetJointVelocityInUUnits(float Velocity)
{
	Constraint->SetJointVelocityInUUnits(Velocity);
}

void URJoint::SetJointEffort(float Effort)
{
	Constraint->SetJointEffort(Effort);
}

void URJoint::SetJointEffortFromROS(float Effort)
{

	Constraint->SetJointEffort(Effort);
}

      // Constraint->AttachToComponent(Child->GetCollision(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
      // Constraint->AddRelativeLocation(Pose.GetLocation());
      // Constraint->AddRelativeRotation(Pose.GetRotation());
