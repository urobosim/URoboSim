// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann (neumann.michael@uni-bremen.de)

#pragma once

#include "Physics/RModel.h"
#include "SDF/SDFJoint.h"
#include "SDF/SDFLink.h"
// clang-format off
#include "RLinkBuilder.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URLinkBuilder : public UObject
{
  GENERATED_BODY()

public:
  virtual void SetWorldPose(const FTransform &InWorldPose) { WorldPose = InWorldPose; }
  virtual bool LoadSDF(USDFLink *&SDFLink);

public:
  UPROPERTY()
  ARModel *Model;

protected:
  virtual void SetNewLink(USDFLink *&SDFLink);
  virtual void SetVisualMeshes(USDFLink *&SDFLink);
  virtual void SetCollisionMeshes(USDFLink *&SDFLink);
  virtual void SetInertial(USDFLinkInertial *&InInertial);
  virtual void SetCollisionProfile(const bool &bSelfColide);
  virtual void SetPose(const FTransform &Pose);

protected:
  UPROPERTY()
  URLink *Link;

  UPROPERTY()
  FTransform WorldPose;
};