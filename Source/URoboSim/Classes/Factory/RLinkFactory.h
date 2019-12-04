// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann (neumann.michael@uni-bremen.de)

#pragma once

#include "CoreMinimal.h"
#include "Physics/RLink.h"
#include "Physics/RModel.h"
#include "SDF/SDFLink.h"
#include "RLinkFactory.generated.h"


UCLASS()
class UROBOSIM_API URLinkBuilder : public UObject
{
  GENERATED_BODY()

public:

  virtual void Init(UObject* InOuter, USDFLink* InLinkDescription);
  virtual URLink* NewLink();

  UPROPERTY()
    ARModel* Model;
protected:


  UPROPERTY()
    USDFLink* LinkDescription;

  UPROPERTY()
    URLink* Link;

  FTransform LinkPose;

  void SetPose(FTransform InPose);
  void SetPose(FVector InLocation, FQuat InRotation);

  virtual void SetVisuals();
  virtual void SetVisual(USDFVisual* InVisual);
  virtual void SetCollisions();
  virtual void SetCollision(USDFCollision* InCollision);
  virtual void SetInertial(USDFLinkInertial* InInertial);
  virtual void SetCollisionProfile(bool InSelfColide);
  virtual void SetSimulateGravity(bool InUseGravity);
  virtual bool CreateCollisionForMesh(UStaticMesh* OutMesh, ESDFGeometryType Type);

};

// UCLASS()
// class UROBOSIM_API URVirtualLinkBuilder : public URLinkBuilder
// {
//   GENERATED_BODY()

//  protected:

//   virtual void SetCollisions() override;
//   virtual void SetVisuals() override;
// };

UCLASS()
class UROBOSIM_API URLinkFactory : public UObject
{
  GENERATED_BODY()
 public:

    URLink* Load(UObject* InOuter, USDFLink* InLinkDescription);

protected:

  virtual URLinkBuilder* CreateBuilder(USDFLink* InLinkDescription);

  UPROPERTY()
    URLinkBuilder* LinkBuilder;

};
