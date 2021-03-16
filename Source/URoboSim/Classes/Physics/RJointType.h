// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "PhysicsEngine/PhysicsConstraintComponent.h"
// clang-format on
#include "RJointType.generated.h"
// clang-format off

UCLASS(BlueprintType, Category = "JointType")
class UROBOSIM_API URJointType : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "JointType")
	float InitialPosition;

	UPROPERTY(VisibleAnywhere, Category = "JointType")
	FVector Axis;

	UPROPERTY(EditAnywhere, Category = "JointType")
	float Lower;

	UPROPERTY(EditAnywhere, Category = "JointType")
	float Upper;

	UPROPERTY(EditAnywhere, Category = "JointType")
	UPhysicsConstraintComponent* Constraint;
};
