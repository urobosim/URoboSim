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
	FVector ParentAngularVelocity = Parent->GetCollisionMeshes()[0]->GetPhysicsAngularVelocityInDegrees();
	FVector ChildAngularVelocity = Child->GetCollisionMeshes()[0]->GetPhysicsAngularVelocityInDegrees();
	FVector JointAngularVelocity = Child->GetCollisionMeshes()[0]->GetComponentRotation().UnrotateVector(ChildAngularVelocity - ParentAngularVelocity);
	JointAngularVelocity.Z *= -1;
	return FMath::RadiansToDegrees(FVector::DotProduct(JointAngularVelocity, Type->Axis));
}

const float URPrismaticJoint::GetVelocity() const
{
	FVector ParentLinearVelocity = Parent->GetCollisionMeshes()[0]->GetPhysicsLinearVelocity();
	FVector ChildLinearVelocity = Child->GetCollisionMeshes()[0]->GetPhysicsLinearVelocity();
	FVector JointLinearVelocity = Child->GetCollisionMeshes()[0]->GetComponentRotation().UnrotateVector(ChildLinearVelocity - ParentLinearVelocity);
	JointLinearVelocity.Z *= -1;
	return FVector::DotProduct(JointLinearVelocity, Type->Axis);
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
	float DeltaPosition = Position - GetPosition();
	Child->GetCollisionMeshes()[0]->AttachToComponent(Parent->GetCollisionMeshes()[0], FAttachmentTransformRules::KeepWorldTransform);
	FVector AxisInWorldFrame = Type->Constraint->GetComponentRotation().RotateVector(Type->Axis);
	FRotator DeltaRotationInWorldFrame = UKismetMathLibrary::RotatorFromAxisAndAngle(AxisInWorldFrame, -DeltaPosition);
	Child->GetCollisionMeshes()[0]->AddWorldRotation(DeltaRotationInWorldFrame);
}

void URPrismaticJoint::SetPosition(const float &Position)
{
	float DeltaPosition = Position - GetPosition();
	Child->GetCollisionMeshes()[0]->AttachToComponent(Parent->GetCollisionMeshes()[0], FAttachmentTransformRules::KeepWorldTransform);
	FVector AxisInWorldFrame = Type->Constraint->GetComponentRotation().RotateVector(Type->Axis);
	FVector DeltaLocationInWorldFrame = AxisInWorldFrame * DeltaPosition;
	Child->GetCollisionMeshes()[0]->AddWorldOffset(DeltaLocationInWorldFrame);
}

void URContinuousJoint::SetTargetPosition(const float &TargetPosition)
{
	Type->Constraint->SetAngularOrientationTarget(UKismetMathLibrary::RotatorFromAxisAndAngle(Type->Axis, TargetPosition));
	Child->GetCollisionMeshes()[0]->WakeRigidBody();
}

void URPrismaticJoint::SetTargetPosition(const float &TargetPosition)
{
	Type->Constraint->SetLinearPositionTarget(Type->Axis * -TargetPosition);
	Child->GetCollisionMeshes()[0]->WakeRigidBody();
}

void URContinuousJoint::SetTargetVelocity(const float &TargetVelocity)
{
	Type->Constraint->SetAngularVelocityTarget(Type->Axis * -TargetVelocity / 360.f);
	Child->GetCollisionMeshes()[0]->WakeRigidBody();
}

void URPrismaticJoint::SetTargetVelocity(const float &TargetVelocity)
{
	Type->Constraint->SetLinearVelocityTarget(Type->Axis * -TargetVelocity);
	Child->GetCollisionMeshes()[0]->WakeRigidBody();
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