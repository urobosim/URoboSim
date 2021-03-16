// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Physics/RJoint.h"
#include "Physics/RLink.h"
#include "Physics/RModel.h"
#include "RStaticMeshComponent.h"


// Sets default values for this component's properties
URJoint::URJoint()
{
  bBreakEnabled = false;
  bActuate = false;
}


float URJoint::GetEncoderValue()
{
  return Constraint->Encoder->GetValue();
}

void URJoint::UpdateEncoder()
{
  Constraint->UpdateEncoderValue(GetJointPosition());
}

void URJoint::EnableMotor(bool InEnable)
{
  Constraint->EnableMotor(InEnable);
}

void URJoint::UpdateVelocity(float InDeltaTime)
{
  if(bActuate)
  {
    Constraint->UpdateJointVelocity(InDeltaTime);
  }

  Child->UpdateVelocity(InDeltaTime);
}

void URJoint::SetParentChild(URLink* InParent, URLink* InChild)
{
  Child = InChild;
  Parent = InParent;

  Constraint->SetParentChild(Parent->GetCollision(), Child->GetCollision());
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

void URJoint::SetMotorJointState(float Position, float Velocity)
{
	Constraint->SetMotorJointState(Position, Velocity);
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
