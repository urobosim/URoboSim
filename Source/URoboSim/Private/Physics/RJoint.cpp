// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Physics/RJoint.h"
#include "Physics/RLink.h"
#include "Physics/RModel.h"
#include "Physics/RStaticMeshComponent.h"

// Sets default values for this component's properties
URJoint::URJoint()
{
  bBreakEnabled = false;
  bActuate = false;
}

bool URJoint::IsTickable() const
{
  return true;
}

TStatId URJoint::GetStatId() const
{
  return TStatId();
}

void URJoint::Tick(float DeltaTime)
{
  if(Constraint)
    {
      UpdateEncoder();
      UE_LOG(LogTemp, Error, TEXT("%s Constraint  ready"), *GetName());
    }
  else
    {
      UE_LOG(LogTemp, Error, TEXT("%s Constraint not ready in tick joint"), *GetName());
    }
}

float URJoint::GetEncoderValue()
{
  return Constraint->Encoder->GetValue();
}

void URJoint::UpdateEncoder()
{
  Constraint->UpdateEncoderValue(GetJointPosition());
}

void URJoint::SetDrive(const FEnableDrive &EnableDrive)
{
  Constraint->SetDrive(EnableDrive);
}

void URJoint::SetParentChild(URLink* InParent, URLink* InChild)
{
  Child = InChild;
  Parent = InParent;

  Constraint->SetParentChild(Parent->GetCollision(), Child->GetCollision());
}

void URJoint::SetParentChild(AActor* InParent, AActor* InChild)
{
  if(!InChild || !InParent)
    {
      return;
    }
  Constraint->SetParentChild(Cast<UStaticMeshComponent>(InParent->GetRootComponent()), Cast<UStaticMeshComponent>(InChild->GetRootComponent()));
}

void URJoint::SetMotorJointState(const FJointState &JointState)
{
  Constraint->SetMotorJointState(JointState.JointPosition, JointState.JointVelocity);
}

FJointState URJoint::GetJointState()
{
  return FJointState(GetJointPosition(), GetJointVelocity());
}

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

void URJoint::SetJointPosition(float Angle, FHitResult * OutSweepHitResult)
{
	Constraint->SetJointPosition(Angle, OutSweepHitResult);
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
