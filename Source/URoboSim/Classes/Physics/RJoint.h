// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "Physics/RJointType.h"
#include "RLink.h"
#include "SDF/SDFJoint.h"
// clang-format off
#include "RJoint.generated.h"
// clang-format on

USTRUCT()
struct FEnableDrive
{
  GENERATED_BODY()

public:
  FEnableDrive() : FEnableDrive(true, true) {}
  FEnableDrive(const bool &bPositionDrive, const bool &bVelocityDrive) : bPositionDrive(bPositionDrive), bVelocityDrive(bVelocityDrive), PositionStrength(1E5), VelocityStrength(5E3), MaxForce(1E10) {}

  UPROPERTY(EditAnywhere)
  bool bPositionDrive;

  UPROPERTY(EditAnywhere)
  bool bVelocityDrive;

  UPROPERTY(EditAnywhere)
  float PositionStrength;

  UPROPERTY(EditAnywhere)
  float VelocityStrength;

  UPROPERTY(EditAnywhere)
  float MaxForce;
};

USTRUCT()
struct FJointState
{
  GENERATED_BODY()

public:
  FJointState() : FJointState(0.f, 0.f) {}
  FJointState(const float &JointPosition, const float &JointVelocity) : JointPosition(JointPosition), JointVelocity(JointVelocity) {}

  UPROPERTY(EditAnywhere)
  float JointPosition;

  UPROPERTY(EditAnywhere)
  float JointVelocity;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URJoint : public UObject
{
  GENERATED_BODY()

public:
  // Called every frame
  virtual void Tick(float DeltaTime);

public:
  virtual void Init();

  virtual const FJointState GetJointState() const { return JointState; }

  virtual void SetJointType(const USDFJoint *InSDFJoint);

  virtual void SetParentChild(URLink *&InParent, URLink *&InChild)
  {
    Parent = InParent;
    Child = InChild;
  }

  virtual void SetConstraint(UPhysicsConstraintComponent *&InConstraint) { Type->Constraint = InConstraint; }

  virtual void SetJointAxis(const FVector &InJointAxis) { Type->Axis = InJointAxis; }

  virtual URLink *GetParent() const { return Parent; }

  virtual URLink *GetChild() const { return Child; }

  virtual const URJointType *GetType() const { return Type; }

  virtual void SetDrive(const FEnableDrive &EnableDrive);

  virtual void SetTargetPosition(const float &TargetPosition);

  virtual void SetTargetVelocity(const float &TargetVelocity);

protected:
  virtual const float GetPosition();

  virtual const FTransform GetChildPoseInJointFrame() const;

protected:
  UPROPERTY(EditAnywhere)
  URLink *Parent;

  UPROPERTY(EditAnywhere)
  URLink *Child;

  UPROPERTY(EditAnywhere)
  URJointType *Type;

  /* Kinematic values */
protected:
  UPROPERTY(VisibleAnywhere)
  FTransform InitChildPoseInJointFrame;

  UPROPERTY(VisibleAnywhere)
  FVector Position;

  UPROPERTY(VisibleAnywhere)
  FJointState JointState;

  UPROPERTY(VisibleAnywhere)
  FVector Velocity;
};
