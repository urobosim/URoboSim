// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Physics/RJoint.h"
#include "Physics/RLink.h"
#include "Physics/RModel.h"
#include "Physics/RStaticMeshComponent.h"

// Sets default values for this component's properties
URJoint::URJoint()
{
  PrimaryComponentTick.bCanEverTick = true;
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

// bool URJoint::IsTickable() const
// {
//   // Tick only if we are both NOT a template and if we are specifically not in-editor-before-beginplay is called.
//   return (!IsTemplate(RF_ClassDefaultObject)) && !(GIsEditor && !GWorld->HasBegunPlay());
// }

void URJoint::Break()
{
  Child->SetPhysicsLinearVelocity(FVector(0, 0, 0));
  Child->SetPhysicsAngularVelocityInDegrees(FVector(0, 0, 0));
  for(auto &ChildJoint : Child->GetJoints())
    {
      ChildJoint->Break();
    }
}

// TStatId URJoint::GetStatId() const
// {
//   return TStatId();
// }

void URJoint::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  if(Constraint)
    {
      UpdateVelocity(DeltaTime);
      UpdateEncoder();
    }
}

float URJoint::GetEncoderValue()
{
  if(Constraint->Encoder)
    {
      return Constraint->Encoder->GetValue();
    }
  else
    {
      return 0;
    }
}

void URJoint::UpdateEncoder()
{
  Constraint->UpdateEncoderValue(GetJointPosition());
}

void URJoint::UpdateVelocity(float InDeltaTime)
{
  JointVelocity = (GetJointPosition() - OldJointValue) / InDeltaTime;
  OldJointValue = GetJointPosition();
}

void URJoint::SetSimulatePhysics(bool bEnablePhysics)
{
  Child->SetSimulatePhysics(bEnablePhysics);
  Constraint->InitComponentConstraint();
  for(auto& MimicJoint : MimicJointList)
    {
      MimicJoint.MimicJoint->SetSimulatePhysics(bEnablePhysics);
    }

  if (Cast<UPrimitiveComponent>(Parent->GetAttachParent()))
		{
      if(!Parent->GetCollision())
        {
          Parent->SetSimulatePhysics(bEnablePhysics);
        }
    }
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

  Constraint->SetParentChild(Parent, Child);
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
      // MimicState.JointPosition = JointState.JointPosition*MimicJoint.Multiplier;
      // MimicState.JointVelocity = JointState.JointVelocity*MimicJoint.Multiplier;
      MimicState.JointPosition = GetJointPosition()*MimicJoint.Multiplier;
      MimicState.JointVelocity = 0;

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
	// return Constraint->GetJointVelocity();
	return JointVelocity;
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
