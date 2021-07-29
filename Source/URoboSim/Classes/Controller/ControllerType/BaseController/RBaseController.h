// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "UObject/ConstructorHelpers.h"
#include "Containers/Queue.h"
#include "Controller/RController.h"
#include "Conversions.h"
#include "Physics/RJoint.h"
#include "Physics/RModel.h"
#include "ROSUtilities.h"
// clang-format off
#include "RBaseController.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URBaseControllerParameter : public URControllerParameter
{
  GENERATED_BODY()

public:
	URBaseControllerParameter()
	{
		BaseName = TEXT("base_footprint");
	}

public:
	UPROPERTY(EditAnywhere)
	FString BaseName;
};

UCLASS()
class UROBOSIM_API URBaseController : public URController
{
	GENERATED_BODY()
public:
	URBaseController();

	virtual void Init() override;
	virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) override;
	virtual void MoveLinear(FVector InVelocity, float InDeltaTime);
	virtual void MoveLinear(FVector InVelocity);
	virtual void MoveLinearToWorld(FVector InVelocity, float InDeltaTime);
	virtual void Turn(float InVelocity, float InDeltaTime);
	virtual void Turn(float InVelocity);

	virtual void SetLocation(FVector InPosition);
	virtual void SetRotation(FRotator InRotator);
	virtual void SetTransform(FTransform InTransform);
	virtual void AddRelativeLocation(URLink* InLink, FVector InPosition);
	virtual void AddRelativeRotation(URLink* InLink, FRotator InRotation);
	virtual void SetLocationAndRotation(FVector InPosition, FRotator InRotation);

	virtual void Tick(const float &InDeltaTime) override;

	virtual TArray<double> GetOdomPositionStates();
	virtual TArray<double> GetOdomVelocityStates();

	UPROPERTY(EditAnywhere)
	FString BaseName;
protected:

	virtual void TurnTick(float InDeltaTime);
	virtual void MoveLinearTick(float InDeltaTime);
	virtual void CalculateOdomStates(float InDeltaTime);

	UPROPERTY(EditAnywhere)
	bool bIsKinematic;

	UPROPERTY()
	float AngularVelocity;

	UPROPERTY(EditAnywhere)
	float HackRotationFactor = 1;

	UPROPERTY(EditAnywhere)
	float HackLinearFactor = 1;

	UPROPERTY()
	FVector LinearVelocity;

	UPROPERTY()
	TArray<double> OdomPositionStates;

	UPROPERTY()
	TArray<double> OdomVelocityStates;

	UPROPERTY()
	FTransform TargetPose;

	UPROPERTY()
	float MaxLinearVelocity;

	UPROPERTY()
	float MaxAngularVelocity;
};

UCLASS()
class UROBOSIM_API URBaseControllerKinematic : public URBaseController
{
    GENERATED_BODY()
public:

  virtual void TurnTick(float InDeltaTime) override;
  virtual void MoveLinearTick(float InDeltaTime) override;
  virtual void SetLocation(FVector InPosition) override;
  virtual void SetRotation(FRotator InRotator) override;

};
