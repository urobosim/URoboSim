// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Physics/RModel.h"
#include "ConstructorHelpers.h"
#include "Physics/RJoint.h"
#include "ROSUtilities.h"
#include "Containers/Queue.h"
#include "Conversions.h"
#include "RBaseController.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URBaseController : public URController
{
    GENERATED_BODY()
public:
	URBaseController();

	virtual void Init(ARModel* InModel) override;
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

	virtual void Tick(float InDeltaTime) override;

	virtual TArray<double> GetOdomPositionStates();
	virtual TArray<double> GetOdomVelocityStates();

	UPROPERTY(EditAnywhere)
	FString BaseName;
protected:

	virtual void TurnTick(float InDeltaTime);
	virtual void MoveLinearTick(float InDeltaTime);
	virtual void CalculateOdomStates(float InDeltaTime);

	UPROPERTY()
	ARModel* Model;

	UPROPERTY(EditAnywhere)
	bool bIsKinematic;

	UPROPERTY()
	float AngularVelocity;

	UPROPERTY()
	FVector LinearVelocity;

	UPROPERTY()
	TArray<double> OdomPositionStates;

	UPROPERTY()
	TArray<double> OdomVelocityStates;
};
