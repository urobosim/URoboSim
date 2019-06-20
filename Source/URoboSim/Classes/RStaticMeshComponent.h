// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once
#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "RStaticMeshComponent.generated.h"

UCLASS()
class UROBOSIM_API URStaticMeshComponent : public UStaticMeshComponent
{
    GENERATED_BODY()

public:
	UPROPERTY()
	FVector NextTickLinearVelocity;

	UPROPERTY()
	FVector NextTickAngularVelocity;

	virtual void SetNextVelocities();
};
