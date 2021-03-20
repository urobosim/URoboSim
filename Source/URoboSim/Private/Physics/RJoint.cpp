// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Physics/RJoint.h"
#include "Conversions.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJoint, Log, All);

void URJoint::SetJointType(const USDFJoint *InSDFJoint)
{
	Type = NewObject<URJointType>(this, *InSDFJoint->Type);
	Type->InitialPosition = InSDFJoint->Axis->InitialPosition;
	Type->Lower = InSDFJoint->Axis->Lower;
	Type->Upper = InSDFJoint->Axis->Upper;
}

void URJoint::Tick(float DeltaTime)
{
	Velocity = Child->GetCollisionMeshes()[0]->GetPhysicsAngularVelocityInDegrees();

	const float CurrentPosition = GetPosition();
	JointState.JointVelocity = (CurrentPosition - JointState.JointPosition) / DeltaTime;
	JointState.JointPosition = CurrentPosition;
}

// Called when the game starts or when spawned
void URJoint::BeginPlay()
{
	InitChildPoseInJointFrame = GetChildPoseInJointFrame();
}

const float URJoint::GetPosition()
{
	FTransform DeltaPoseInJointFrame = InitChildPoseInJointFrame.Inverse() * GetChildPoseInJointFrame();
	if (Type->GetName().Equals("revolute") || Type->GetName().Equals("continuous"))
	{
		FQuat DeltaRotationInJointFrame = DeltaPoseInJointFrame.GetRotation();
		return FVector::DotProduct(DeltaRotationInJointFrame.Euler(), Type->Axis * FVector(1.f, 1.f, -1.f));
	}
	else if (Type->GetName().Equals("prismatic"))
	{
		FVector DeltaPositionInJointFrame = DeltaPoseInJointFrame.GetLocation();
		return FVector::DotProduct(DeltaPositionInJointFrame, Type->Axis);
	}
	else
	{
		UE_LOG(LogRJoint, Error, TEXT("Unknown Type %s of Joint %s"), *Type->GetName(), *GetName())
		return 0.f;
	}
}

const FTransform URJoint::GetChildPoseInJointFrame() const
{
	FTransform ChildPose = Child->GetPose();
	FTransform JointPose = Type->Constraint->GetComponentTransform();
	return ChildPose * JointPose.Inverse();
}

void URJoint::SetTargetPosition(const float &TargetPosition)
{
	if (Type->Constraint->ConstraintInstance.ProfileInstance.AngularDrive.AngularDriveMode == EAngularDriveMode::TwistAndSwing)
	{
		Type->Constraint->SetAngularOrientationTarget(UKismetMathLibrary::RotatorFromAxisAndAngle(Type->Axis, TargetPosition));
		Child->GetCollisionMeshes()[0]->WakeRigidBody();
	}
	else if (Type->Constraint->ConstraintInstance.ProfileInstance.LinearDrive.IsPositionDriveEnabled())
	{
		Type->Constraint->SetLinearPositionTarget(Type->Axis * -TargetPosition);
		Child->GetCollisionMeshes()[0]->WakeRigidBody();
	}
	else
	{
		float DeltaPosition = TargetPosition - GetPosition();
		Child->GetCollisionMeshes()[0]->AttachToComponent(Parent->GetCollisionMeshes()[0], FAttachmentTransformRules::KeepWorldTransform);
		if (Type->GetName().Equals("revolute") || Type->GetName().Equals("continuous"))
		{
			FVector AxisInWorldFrame = Type->Constraint->GetComponentRotation().RotateVector(Type->Axis);
			FRotator DeltaRotationInWorldFrame = UKismetMathLibrary::RotatorFromAxisAndAngle(AxisInWorldFrame, -DeltaPosition);
			Child->GetCollisionMeshes()[0]->AddWorldRotation(DeltaRotationInWorldFrame);
		}
		else if (Type->GetName().Equals("prismatic"))
		{
			FVector AxisInWorldFrame = Type->Constraint->GetComponentRotation().RotateVector(Type->Axis);
			FVector DeltaLocationInWorldFrame = AxisInWorldFrame * DeltaPosition;
			Child->GetCollisionMeshes()[0]->AddWorldOffset(DeltaLocationInWorldFrame);
		}
	}
}

void URJoint::SetTargetVelocity(const float &TargetVelocity)
{
	if (Type->Constraint->ConstraintInstance.ProfileInstance.AngularDrive.AngularDriveMode == EAngularDriveMode::TwistAndSwing)
	{
		Type->Constraint->SetAngularVelocityTarget(Type->Axis * TargetVelocity);
		Child->GetCollisionMeshes()[0]->WakeRigidBody();
	}
	else if (Type->Constraint->ConstraintInstance.ProfileInstance.LinearDrive.IsPositionDriveEnabled())
	{
		Type->Constraint->SetLinearVelocityTarget(Type->Axis * TargetVelocity);
		Child->GetCollisionMeshes()[0]->WakeRigidBody();
	}
}

void URJoint::SetDrive(const FEnableDrive &EnableDrive)
{
	if (Type->GetName().Equals("revolute") || Type->GetName().Equals("continuous"))
	{
		Type->Constraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
		Type->Constraint->SetAngularDriveParams(EnableDrive.PositionStrength, EnableDrive.VelocityStrength, EnableDrive.MaxForce);
		if (Type->Axis.GetAbs().Equals(FVector::ForwardVector))
		{
			Type->Constraint->SetAngularOrientationDrive(false, EnableDrive.bPositionDrive);
			Type->Constraint->SetAngularVelocityDrive(false, EnableDrive.bVelocityDrive);
		}
		else
		{
			Type->Constraint->SetAngularOrientationDrive(EnableDrive.bPositionDrive, false);
			Type->Constraint->SetAngularVelocityDrive(EnableDrive.bVelocityDrive, false);
		}
	}
	else if (Type->GetName().Equals("prismatic"))
	{
		Type->Constraint->SetLinearDriveParams(EnableDrive.PositionStrength, EnableDrive.VelocityStrength, EnableDrive.MaxForce);
		if (Type->Axis.GetAbs().Equals(FVector::ForwardVector))
		{
			Type->Constraint->SetLinearPositionDrive(EnableDrive.bPositionDrive, false, false);
			Type->Constraint->SetLinearVelocityDrive(EnableDrive.bVelocityDrive, false, false);
		}
		else if (Type->Axis.GetAbs().Equals(FVector::RightVector))
		{
			Type->Constraint->SetLinearPositionDrive(false, EnableDrive.bPositionDrive, false);
			Type->Constraint->SetLinearVelocityDrive(false, EnableDrive.bVelocityDrive, false);
		}
		else
		{
			Type->Constraint->SetLinearPositionDrive(false, false, EnableDrive.bPositionDrive);
			Type->Constraint->SetLinearVelocityDrive(false, false, EnableDrive.bVelocityDrive);
		}
	}
}