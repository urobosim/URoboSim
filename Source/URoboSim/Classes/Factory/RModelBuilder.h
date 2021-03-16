// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "Factory/RJointBuilder.h"
#include "Factory/RLinkBuilder.h"
#include "Physics/RModel.h"
#include "SDF/SDFModel.h"

// clang-format off
#include "RModelBuilder.generated.h"
// clang-format on

class USDFModel;
class USDFJoint;
class USDFLink;

UCLASS()
class UROBOSIM_API URModelBuilder : public UObject
{
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  URModelBuilder();

  UPROPERTY()
  ARModel *Model;

  // Load model
  virtual void LoadSDF(USDFModel *&SDFModel);

protected:
  UPROPERTY()
  URJointBuilder *JointBuilder;

  UPROPERTY()
  URLinkBuilder *LinkBuilder;

  virtual void SwapBaseLinkToFirstIndex(USDFModel *&SDFModel);
  virtual void LoadLinks(USDFModel *&SDFModel);
  virtual void LoadJoints(USDFModel *&SDFModel);
  virtual void LockBaseLink();
};
