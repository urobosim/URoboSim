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
	JointState.JointVelocity = GetVelocity();
	JointState.JointPosition = GetPosition();
}

void URJoint::Init()
{
	InitChildPoseInJointFrame = GetChildPoseInJointFrame();
}

const FJointState URJoint::GetJointStateInROSUnit() const
{
	FJointState JointStateInROSUnit;
	if (Type->GetName().Equals("revolute") || Type->GetName().Equals("continuous"))
	{
		JointStateInROSUnit.JointPosition = FMath::DegreesToRadians(JointState.JointPosition);
		JointStateInROSUnit.JointVelocity = FMath::DegreesToRadians(JointState.JointVelocity);
	}
	else if (Type->GetName().Equals("prismatic"))
	{
		JointStateInROSUnit.JointPosition = FConversions::CmToM((float)JointState.JointPosition);
		JointStateInROSUnit.JointVelocity = FConversions::CmToM((float)JointState.JointVelocity);
	}
	return JointStateInROSUnit;
}

const float URJoint::GetVelocity() const
{
	if (Type->GetName().Equals("revolute") || Type->GetName().Equals("continuous"))
	{
		FVector ParentVelocity = Parent->GetCollisionMeshes()[0]->GetPhysicsAngularVelocityInDegrees();
		FVector ChildVelocity = Child->GetCollisionMeshes()[0]->GetPhysicsAngularVelocityInDegrees();
		FVector JointVelocity = Child->GetCollisionMeshes()[0]->GetComponentRotation().UnrotateVector(ChildVelocity - ParentVelocity);
		JointVelocity.Z *= -1;
		return FMath::RadiansToDegrees(FVector::DotProduct(JointVelocity, Type->Axis));
	}
	else if (Type->GetName().Equals("prismatic"))
	{
		FVector ParentVelocity = Parent->GetCollisionMeshes()[0]->GetPhysicsLinearVelocity();
		FVector ChildVelocity = Child->GetCollisionMeshes()[0]->GetPhysicsLinearVelocity();
		FVector JointVelocity = Child->GetCollisionMeshes()[0]->GetComponentRotation().UnrotateVector(ChildVelocity - ParentVelocity);
		JointVelocity.Z *= -1;
		return FVector::DotProduct(JointVelocity, Type->Axis);
	}
	else
	{
		UE_LOG(LogRJoint, Error, TEXT("Unknown Type %s of Joint %s"), *Type->GetName(), *GetName())
		return 0.f;
	}
}

const float URJoint::GetPosition() const
{
	FTransform DeltaPoseInJointFrame = InitChildPoseInJointFrame.Inverse() * GetChildPoseInJointFrame();
	if (Type->GetName().Equals("revolute") || Type->GetName().Equals("continuous"))
	{
		FRotator DeltaRotationInJointFrame = DeltaPoseInJointFrame.GetRotation().Rotator();
		DeltaRotationInJointFrame.Yaw *= -1;
		return FVector::DotProduct(DeltaRotationInJointFrame.Euler(), Type->Axis);
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

void URJoint::SetPositionInROSUnit(const float &Position)
{
	if (Type->GetName().Equals("revolute") || Type->GetName().Equals("continuous"))
	{
		SetPosition(FMath::RadiansToDegrees(Position));
	}
	else if (Type->GetName().Equals("prismatic"))
	{
		SetPosition(FConversions::MToCm(Position));
	}
}

void URJoint::SetTargetPositionInROSUnit(const float &TargetPosition)
{
	if (Type->GetName().Equals("revolute") || Type->GetName().Equals("continuous"))
	{
		SetTargetPosition(FMath::RadiansToDegrees(TargetPosition));
	}
	else if (Type->GetName().Equals("prismatic"))
	{
		SetTargetPosition(FConversions::MToCm(TargetPosition));
	}
}

void URJoint::SetTargetVelocityInROSUnit(const float &TargetVelocity)
{
	if (Type->GetName().Equals("revolute") || Type->GetName().Equals("continuous"))
	{
		SetTargetVelocity(FMath::RadiansToDegrees(TargetVelocity));
	}
	else if (Type->GetName().Equals("prismatic"))
	{
		SetTargetVelocity(FConversions::MToCm(TargetVelocity));
	}
}

void URJoint::SetPosition(const float &Position)
{
	float DeltaPosition = Position - GetPosition();
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

void URJoint::SetTargetPosition(const float &TargetPosition)
{
	if (Type->Constraint->ConstraintInstance.ProfileInstance.AngularDrive.IsOrientationDriveEnabled())
	{
		Type->Constraint->SetAngularOrientationTarget(UKismetMathLibrary::RotatorFromAxisAndAngle(Type->Axis, TargetPosition));
		Child->GetCollisionMeshes()[0]->WakeRigidBody();
	}
	else if (Type->Constraint->ConstraintInstance.ProfileInstance.LinearDrive.IsPositionDriveEnabled())
	{
		Type->Constraint->SetLinearPositionTarget(Type->Axis * -TargetPosition);
		Child->GetCollisionMeshes()[0]->WakeRigidBody();
	}
}

void URJoint::SetTargetVelocity(const float &TargetVelocity)
{
	if (Type->Constraint->ConstraintInstance.ProfileInstance.AngularDrive.IsVelocityDriveEnabled())
	{
		Type->Constraint->SetAngularVelocityTarget(Type->Axis * TargetVelocity / 360.f);
		Child->GetCollisionMeshes()[0]->WakeRigidBody();
	}
	else if (Type->Constraint->ConstraintInstance.ProfileInstance.LinearDrive.IsVelocityDriveEnabled())
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