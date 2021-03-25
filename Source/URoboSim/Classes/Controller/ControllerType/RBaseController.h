// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Controller/RController.h"
#include "Physics/RModel.h"
// clang-format off
#include "RBaseController.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URBaseController : public URController
{
	GENERATED_BODY()

public:
	URBaseController();

public:
  virtual void Tick(float DeltaTime) override;

	virtual void Init() override;

public:
	virtual void MoveLinear(const FVector &InVelocity);

  virtual void MoveAngular(const float &InVelocity);

  virtual TArray<float> GetOdomPosition() const;

	virtual TArray<float> GetOdomVelocity() const;
	
	// virtual void SetLocation(FVector InPosition);
	// virtual void SetRotation(FRotator InRotator);
	// virtual void SetTransform(FTransform InTransform);
	// virtual void AddRelativeLocation(URLink* InLink, FVector InPosition);
	// virtual void AddRelativeRotation(URLink* InLink, FRotator InRotation);
	// virtual void SetLocationAndRotation(FVector InPosition, FRotator InRotation);
	// virtual void MoveLinearToWorld(FVector InVelocity, float InDeltaTime);

protected:
	virtual void MoveAngularTick(float InDeltaTime);

	virtual void MoveLinearTick(float InDeltaTime);

	virtual void CalculateOdomStates(float InDeltaTime);

protected:
  UPROPERTY()
  URLink *BaseLink;

  UPROPERTY()
	FTransform TargetPose;

  UPROPERTY()
  FTransform BasePose;

  UPROPERTY(EditAnywhere)
	FVector LinearVelocity;

  UPROPERTY(EditAnywhere)
	float AngularVelocity;

  UPROPERTY()
	TArray<float> OdomPosition;

	UPROPERTY()
	TArray<float> OdomVelocity;

	// UPROPERTY(EditAnywhere)
	// bool bIsKinematic;

	// UPROPERTY(EditAnywhere)
	// float HackRotationFactor = 1;

	// UPROPERTY(EditAnywhere)
	// float HackLinearFactor = 1;
};

// UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
// class UROBOSIM_API URBaseControllerKinematic : public URBaseController
// {
//     GENERATED_BODY()
// public:

//   virtual void TurnTick(float InDeltaTime) override;
//   virtual void MoveLinearTick(float InDeltaTime) override;
//   virtual void SetLocation(FVector InPosition) override;
//   virtual void SetRotation(FRotator InRotator) override;
// };
