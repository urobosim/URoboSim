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

void URJoint::BeginDestroy()
{
  if(Constraint->IsValidLowLevel())
    {
      Constraint->ConditionalBeginDestroy();
    }
  Super::BeginDestroy();
}

bool URJoint::IsTickable() const
{
  // Tick only if we are both NOT a template and if we are specifically not in-editor-before-beginplay is called.
  return (!IsTemplate(RF_ClassDefaultObject)) && !(GIsEditor && !GWorld->HasBegunPlay());
}

void URJoint::Break()
{
  Child->GetCollision()->SetPhysicsLinearVelocity(FVector(0, 0, 0));
  Child->GetCollision()->SetPhysicsAngularVelocity(FVector(0, 0, 0));
  for(auto &ChildJoint : Child->GetJoints())
    {
      ChildJoint->Break();
    }
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
  for(auto& MimicJoint : MimicJointList)
    {
      MimicJoint.MimicJoint->SetDrive(EnableDrive);
    }
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

  for(auto& MimicJoint : MimicJointList)
    {
      FJointState MimicState;
      MimicState.JointPosition = JointState.JointPosition*MimicJoint.Multiplier;
      MimicState.JointVelocity = JointState.JointVelocity*MimicJoint.Multiplier;

      MimicJoint.MimicJoint->SetMotorJointState(MimicState);
    }
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

  for(auto& MimicJoint : MimicJointList)
    {
      MimicJoint.MimicJoint->SetJointPosition(Angle*MimicJoint.Multiplier, OutSweepHitResult);
    }
}
void URJoint::SetJointVelocity(float Velocity)
{
  Constraint->SetJointVelocity(Velocity);

  for(auto& MimicJoint : MimicJointList)
    {
      MimicJoint.MimicJoint->SetJointVelocity(Velocity*MimicJoint.Multiplier);
    }
}

void URJoint::SetJointVelocityInUUnits(float Velocity)
{
  Constraint->SetJointVelocityInUUnits(Velocity);
  for(auto& MimicJoint : MimicJointList)
    {
      MimicJoint.MimicJoint->SetJointVelocityInUUnits(Velocity*MimicJoint.Multiplier);
    }
}

void URJoint::SetJointEffort(float Effort)
{
  Constraint->SetJointEffort(Effort);
  for(auto& MimicJoint : MimicJointList)
    {
      MimicJoint.MimicJoint->SetJointEffort(Effort*MimicJoint.Multiplier);
    }
}

void URJoint::SetJointEffortFromROS(float Effort)
{
  Constraint->SetJointEffort(Effort);
  for(auto& MimicJoint : MimicJointList)
    {
      MimicJoint.MimicJoint->SetJointEffort(Effort*MimicJoint.Multiplier);
    }
}
