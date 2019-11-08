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
