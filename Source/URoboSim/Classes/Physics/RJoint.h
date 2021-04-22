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
  UPROPERTY(EditAnywhere)
  bool bPositionDrive = true;

  UPROPERTY(EditAnywhere)
  bool bVelocityDrive = true;

  UPROPERTY(EditAnywhere)
  float PositionStrength = 1E5;

  UPROPERTY(EditAnywhere)
  float VelocityStrength = 1E5;

  UPROPERTY(EditAnywhere)
  float MaxForce = 1E10;
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

UCLASS(ClassGroup = (Abstract), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URJoint : public UObject
{
  GENERATED_BODY()

public:
  // Called every frame
  void Tick(float DeltaTime);

public:
  void Init();

  virtual const FJointState GetJointStateInROSUnit() const { return FJointState(); }

  const FJointState GetJointState() const { return JointState; }

  void SetJointType(const USDFJoint *InSDFJoint);

  void SetParentChild(URLink *&InParent, URLink *&InChild)
  {
    Parent = InParent;
    Child = InChild;
  }

  void SetConstraint(UPhysicsConstraintComponent *&InConstraint) { Type->Constraint = InConstraint; }

  void SetJointAxis(const FVector &InJointAxis) { Type->Axis = InJointAxis; }

  URLink *GetParent() const { return Parent; }

  URLink *GetChild() const { return Child; }

  const URJointType *GetType() const { return Type; }

  virtual void SetDrive(const FEnableDrive &EnableDrive) {}

  virtual void SetPosition(const float &Position) {}

  virtual void SetTargetPositionInROSUnit(const float &TargetPosition) {}

  virtual void SetTargetVelocityInROSUnit(const float &TargetVelocity) {}

  virtual void SetPositionInROSUnit(const float &Position) {}

  virtual void SetTargetPosition(const float &TargetPosition) {}

  virtual void SetTargetVelocity(const float &TargetVelocity) {}

protected:
  const FTransform GetChildPoseInJointFrame() const;

  virtual const float GetPosition() const { return 0.f; }

  virtual const float GetVelocity() const { return 0.f; }

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
  FJointState JointState;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URContinuousJoint : public URJoint
{
  GENERATED_BODY()

public:
  virtual const FJointState GetJointStateInROSUnit() const override;

  virtual void SetDrive(const FEnableDrive &EnableDrive) override;

  virtual void SetPosition(const float &Position) override;

  virtual void SetTargetPositionInROSUnit(const float &TargetPosition) override;

  virtual void SetTargetVelocityInROSUnit(const float &TargetVelocity) override;

  virtual void SetPositionInROSUnit(const float &Position) override;

  virtual void SetTargetPosition(const float &TargetPosition) override;

  virtual void SetTargetVelocity(const float &TargetVelocity) override;

protected:
  virtual const float GetPosition() const override;

  virtual const float GetVelocity() const override;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URRevoluteJoint : public URContinuousJoint
{
  GENERATED_BODY()
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URPrismaticJoint : public URJoint
{
  GENERATED_BODY()

public:
  virtual const FJointState GetJointStateInROSUnit() const override;

  virtual void SetDrive(const FEnableDrive &EnableDrive) override;

  virtual void SetPosition(const float &Position) override;

  virtual void SetTargetPositionInROSUnit(const float &TargetPosition) override;

  virtual void SetTargetVelocityInROSUnit(const float &TargetVelocity) override;

  virtual void SetPositionInROSUnit(const float &Position) override;

  virtual void SetTargetPosition(const float &TargetPosition) override;

  virtual void SetTargetVelocity(const float &TargetVelocity) override;

protected:
  virtual const float GetPosition() const override;

  virtual const float GetVelocity() const override;
};