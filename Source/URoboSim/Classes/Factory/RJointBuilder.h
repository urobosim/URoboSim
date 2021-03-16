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
  UPROPERTY()
  ARModel *Model;

  virtual bool LoadSDF(USDFJoint *&SDFJoint);

protected:
  UPROPERTY()
  URJoint *Joint;

  virtual void SetNewJoint(USDFJoint *&SDFJoint);
  virtual void SetJointParameters(USDFJoint *&SDFJoint);
  virtual void CreateConstraint(USDFJoint *&SDFJoint);
  virtual void RotateConstraintToRefAxis(UPhysicsConstraintComponent *&Constraint, USDFJointAxis *&SDFJointAxis);
  virtual void ConnectToComponents(UPhysicsConstraintComponent *&Constraint, USDFJoint *&SDFJoint);

  // virtual void RotateConstraintToRefAxis();
  // virtual float CalculateRotationOffset();
};