// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Controller/RController.h"
#include "Physics/RModel.h"
// clang-format off
#include "RBaseController.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URBaseControllerParameter : public URControllerParameter
{
  GENERATED_BODY()

};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URBaseController : public URController
{
	GENERATED_BODY()

public:
  virtual void Tick(const float &InDeltaTime) override;

	virtual void Init() override;

public:
	virtual void MoveLinear(const FVector &InVelocity);

  virtual void MoveAngular(const float &InVelocity);

protected:
	virtual void MoveAngularTick(const float &InDeltaTime);

	virtual void MoveLinearTick(const float &InDeltaTime);

protected:
  UStaticMeshComponent *BaseMesh;

	FTransform TargetPose;

  FTransform BasePose;

  UPROPERTY(EditAnywhere)
	FVector LinearVelocity;

  UPROPERTY(EditAnywhere)
	float AngularVelocity;
};