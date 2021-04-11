// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann (neumann.michael@uni-bremen.de)

#pragma once

#include "Physics/RModel.h"
#include "SDF/SDFJoint.h"
// clang-format off
#include "RJointBuilder.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointBuilder : public UObject
{
  GENERATED_BODY()

public:
  ARModel *Model;

  virtual bool LoadSDF(USDFJoint *&SDFJoint);

protected:
  URJoint *Joint;

  virtual void SetNewJoint(USDFJoint *&SDFJoint);

  virtual void SetJointParameters(USDFJoint *&SDFJoint);

  virtual void CreateConstraint(USDFJoint *&SDFJoint);

  virtual void SetupConstraint(UPhysicsConstraintComponent *&Constraint, USDFJointAxis *&SDFJointAxis) {}

  virtual void RotateConstraintToRefAxis(UPhysicsConstraintComponent *&Constraint, USDFJointAxis *&SDFJointAxis);

  virtual void ConnectToComponents(UPhysicsConstraintComponent *&Constraint, USDFJoint *&SDFJoint);
};

UCLASS()
class UROBOSIM_API URContinuousJointBuilder : public URJointBuilder
{
  GENERATED_BODY()

protected:
  virtual void SetNewJoint(USDFJoint *&SDFJoint) override;

  virtual void SetupConstraint(UPhysicsConstraintComponent *&Constraint, USDFJointAxis *&SDFJointAxis) override;
};

UCLASS()
class UROBOSIM_API URRevoluteJointBuilder : public URContinuousJointBuilder
{
  GENERATED_BODY()

protected:
  virtual void SetNewJoint(USDFJoint *&SDFJoint) override;

  virtual void SetupConstraint(UPhysicsConstraintComponent *&Constraint, USDFJointAxis *&SDFJointAxis) override;

  virtual const float CalculateRotationOffset(float &JointLimit, USDFJointAxis *&SDFJointAxis);
};

UCLASS()
class UROBOSIM_API URPrismaticJointBuilder : public URJointBuilder
{
  GENERATED_BODY()

protected:
  virtual void SetNewJoint(USDFJoint *&SDFJoint) override;

  virtual void SetupConstraint(UPhysicsConstraintComponent *&Constraint, USDFJointAxis *&SDFJointAxis) override;
};