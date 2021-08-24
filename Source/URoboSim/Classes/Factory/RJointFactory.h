// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann (neumann.michael@uni-bremen.de)

#pragma once

#include "CoreMinimal.h"
#include "Physics/RJoint.h"
#include "SDF/SDFJoint.h"
#include "RJointFactory.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, hidecategories = Object, editinlinenew)
class UROBOSIM_API URJointBuilder : public UObject
{
  GENERATED_BODY()

public:

  virtual void Init(UObject* InOuter, USDFJoint* InJointDescription);
  virtual URJoint* NewJoint();

protected:

    virtual void SetAxis();
    virtual void SetJointParameters();
    virtual void CreateConstraint();
    virtual void SetJointLimit();
    virtual void RotateConstraintToRefAxis();
    virtual float CalculateRotationOffset();

  UPROPERTY()
    UObject* Outer;

  UPROPERTY()
    USDFJoint* JointDescription;

  UPROPERTY()
    URJoint* Joint;

};

UCLASS(Blueprintable, DefaultToInstanced,   hidecategories = Object, editinlinenew)
class UROBOSIM_API URContiniousJointBuilder : public URJointBuilder
{
  GENERATED_BODY()

protected:
    virtual void SetAxis() override;
    virtual void CreateConstraint() override;

};

UCLASS(Blueprintable, DefaultToInstanced,   hidecategories = Object, editinlinenew)
class UROBOSIM_API URRevoluteJointBuilder : public URContiniousJointBuilder
{
  GENERATED_BODY()

protected:
    virtual void SetAxis() override;
    virtual void CreateConstraint() override;
};

UCLASS(Blueprintable, DefaultToInstanced, hidecategories = Object, editinlinenew)
class UROBOSIM_API URPrismaticJointBuilder : public URJointBuilder
{
  GENERATED_BODY()

protected:
    virtual void SetAxis() override;
    virtual void CreateConstraint() override;
};

UCLASS()
class UROBOSIM_API URJointFactory : public UObject
{
  GENERATED_BODY()

public:

    URJoint* Load(UObject* InOuter, USDFJoint* InJointDescription);

 protected:
  URJointBuilder* CreateBuilder(USDFJoint* InJointDescription);

  UPROPERTY()
    URJointBuilder* JointBuilder;
};
