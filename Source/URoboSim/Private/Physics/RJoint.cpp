// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#include "Physics/RJoint.h"
#include "Conversions.h"
#include "Kismet/KismetMathLibrary.h"

DEFINE_LOG_CATEGORY_STATIC(LogRJoint, Log, All);

void URJoint::Tick(float DeltaTime)
{
	JointState.JointVelocity = GetVelocity();
	JointState.JointPosition = GetPosition();
}

void URJoint::Init()
{
	InitChildPoseInJointFrame = GetChildPoseInJointFrame();
}

void URJoint::SetJointType(const USDFJoint *InSDFJoint)
{
	Type = NewObject<URJointType>(this, *InSDFJoint->Type);
	Type->InitialPosition = InSDFJoint->Axis->InitialPosition;
	Type->Lower = InSDFJoint->Axis->Lower;
	Type->Upper = InSDFJoint->Axis->Upper;
}

const FJointState URContinuousJoint::GetJointStateInROSUnit() const
{
	FJointState JointStateInROSUnit;
	JointStateInROSUnit.JointPosition = FMath::DegreesToRadians(JointState.JointPosition);
	JointStateInROSUnit.JointVelocity = FMath::DegreesToRadians(JointState.JointVelocity);
	return JointStateInROSUnit;
}

const FJointState URPrismaticJoint::GetJointStateInROSUnit() const
{
	FJointState JointStateInROSUnit;
	JointStateInROSUnit.JointPosition = FConversions::CmToM((float)JointState.JointPosition);
	JointStateInROSUnit.JointVelocity = FConversions::CmToM((float)JointState.JointVelocity);
	return JointStateInROSUnit;
}

const float URContinuousJoint::GetVelocity() const
{
	if (Parent->GetRootMesh() && Child->GetRootMesh())
	{
		FVector ParentAngularVelocity = Parent->GetRootMesh()->GetPhysicsAngularVelocityInDegrees();
		FVector ChildAngularVelocity = Child->GetRootMesh()->GetPhysicsAngularVelocityInDegrees();
		FVector JointAngularVelocity = Child->GetRootMesh()->GetComponentRotation().UnrotateVector(ChildAngularVelocity - ParentAngularVelocity);
		JointAngularVelocity.Z *= -1;
		return FMath::RadiansToDegrees(FVector::DotProduct(JointAngularVelocity, Type->Axis));
	}
	else
	{
		UE_LOG(LogRJoint, Error, TEXT("Parent or Child of %s not found"), *GetName())
		return 0.f;
	}
}

const float URPrismaticJoint::GetVelocity() const
{
	if (Parent->GetRootMesh() && Child->GetRootMesh())
	{
		FVector ParentLinearVelocity = Parent->GetRootMesh()->GetPhysicsLinearVelocity();
		FVector ChildLinearVelocity = Child->GetRootMesh()->GetPhysicsLinearVelocity();
		FVector JointLinearVelocity = Child->GetRootMesh()->GetComponentRotation().UnrotateVector(ChildLinearVelocity - ParentLinearVelocity);
		JointLinearVelocity.Z *= -1;
		return FVector::DotProduct(JointLinearVelocity, Type->Axis);
	}
	else
	{
		UE_LOG(LogRJoint, Error, TEXT("Parent or Child of %s not found"), *GetName())
		return 0.f;
	}
}

const float URContinuousJoint::GetPosition() const
{
	FTransform DeltaPoseInJointFrame = InitChildPoseInJointFrame.Inverse() * GetChildPoseInJointFrame();
	FRotator DeltaRotationInJointFrame = DeltaPoseInJointFrame.GetRotation().Rotator();
	DeltaRotationInJointFrame.Yaw *= -1;
	return FVector::DotProduct(DeltaRotationInJointFrame.Euler(), Type->Axis);
}

const float URPrismaticJoint::GetPosition() const
{
	FTransform DeltaPoseInJointFrame = InitChildPoseInJointFrame.Inverse() * GetChildPoseInJointFrame();
	FVector DeltaPositionInJointFrame = DeltaPoseInJointFrame.GetLocation();
	return FVector::DotProduct(DeltaPositionInJointFrame, Type->Axis);
}

const FTransform URJoint::GetChildPoseInJointFrame() const
{
	FTransform ChildPose = Child->GetPose();
	FTransform JointPose = Type->Constraint->GetComponentTransform();
	return ChildPose * JointPose.Inverse();
}

void URContinuousJoint::SetPositionInROSUnit(const float &Position)
{
	SetPosition(FMath::RadiansToDegrees(Position));
}

void URPrismaticJoint::SetPositionInROSUnit(const float &Position)
{
	SetPosition(FConversions::MToCm(Position));
}

void URContinuousJoint::SetTargetPositionInROSUnit(const float &TargetPosition)
{
	SetTargetPosition(FMath::RadiansToDegrees(TargetPosition));
}

void URPrismaticJoint::SetTargetPositionInROSUnit(const float &TargetPosition)
{
	SetTargetPosition(FConversions::MToCm(TargetPosition));
}

void URContinuousJoint::SetTargetVelocityInROSUnit(const float &TargetVelocity)
{
	SetTargetVelocity(FMath::RadiansToDegrees(TargetVelocity));
}

void URPrismaticJoint::SetTargetVelocityInROSUnit(const float &TargetVelocity)
{
	SetTargetVelocity(FConversions::MToCm(TargetVelocity));
}

void URContinuousJoint::SetPosition(const float &Position)
{
	if (Parent->GetRootMesh() && Child->GetRootMesh())
	{
		float DeltaPosition = Position - GetPosition();
		Child->GetRootMesh()->AttachToComponent(Parent->GetRootMesh(), FAttachmentTransformRules::KeepWorldTransform);
		FVector AxisInWorldFrame = Type->Constraint->GetComponentRotation().RotateVector(Type->Axis);
		FRotator DeltaRotationInWorldFrame = UKismetMathLibrary::RotatorFromAxisAndAngle(AxisInWorldFrame, -DeltaPosition);
		Child->GetRootMesh()->AddWorldRotation(DeltaRotationInWorldFrame);
	}
	else
	{
		UE_LOG(LogRJoint, Error, TEXT("Parent or Child of %s not found"), *GetName())
	}
}

void URPrismaticJoint::SetPosition(const float &Position)
{
	if (Parent->GetRootMesh() && Child->GetRootMesh())
	{
		float DeltaPosition = Position - GetPosition();
		Child->GetRootMesh()->AttachToComponent(Parent->GetRootMesh(), FAttachmentTransformRules::KeepWorldTransform);
		FVector AxisInWorldFrame = Type->Constraint->GetComponentRotation().RotateVector(Type->Axis);
		FVector DeltaLocationInWorldFrame = AxisInWorldFrame * DeltaPosition;
		Child->GetRootMesh()->AddWorldOffset(DeltaLocationInWorldFrame);
	}
	else
	{
		UE_LOG(LogRJoint, Error, TEXT("Parent or Child of %s not found"), *GetName())
	}
}

void URContinuousJoint::SetTargetPosition(const float &TargetPosition)
{
	if (Child->GetRootMesh())
	{
		const FRotator TargetPositionRotator = UKismetMathLibrary::RotatorFromAxisAndAngle(Type->Axis, TargetPosition);
		Type->Constraint->SetAngularOrientationTarget(TargetPositionRotator);
		Child->GetRootMesh()->WakeRigidBody();
	}
	else
	{
		UE_LOG(LogRJoint, Error, TEXT("Child of %s not found"), *GetName())
	}
}

void URPrismaticJoint::SetTargetPosition(const float &TargetPosition)
{
	if (Child->GetRootMesh())
	{
		const FVector TargetPositionVector = Type->Axis * -TargetPosition;
		Type->Constraint->SetLinearPositionTarget(TargetPositionVector);
		Child->GetRootMesh()->WakeRigidBody();
	}
	else
	{
		UE_LOG(LogRJoint, Error, TEXT("Child of %s not found"), *GetName())
	}
}

void URContinuousJoint::SetTargetVelocity(const float &TargetVelocity)
{
	if (Child->GetRootMesh())
	{
		Type->Constraint->SetAngularVelocityTarget(Type->Axis * -TargetVelocity / 360.f);
		Child->GetRootMesh()->WakeRigidBody();
	}
	else
	{
		UE_LOG(LogRJoint, Error, TEXT("Child of %s not found"), *GetName())
	}
}

void URPrismaticJoint::SetTargetVelocity(const float &TargetVelocity)
{
	if (Child->GetRootMesh())
	{
		Type->Constraint->SetLinearVelocityTarget(Type->Axis * -TargetVelocity);
		Child->GetRootMesh()->WakeRigidBody();
	}
	else
	{
		UE_LOG(LogRJoint, Error, TEXT("Child of %s not found"), *GetName())
	}
}

void URContinuousJoint::SetDrive(const FEnableDrive &EnableDrive)
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

void URPrismaticJoint::SetDrive(const FEnableDrive &EnableDrive)
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