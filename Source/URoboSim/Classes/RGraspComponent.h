// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
#pragma once

#include "CoreMinimal.h"
#include "RStaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "RGraspComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UROBOSIM_API URGraspComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	URGraspComponent();

	virtual void Init(URStaticMeshComponent* InGripper1, URStaticMeshComponent* InGripper2);
	virtual void Init(URStaticMeshComponent* InGripper);
	// virtual void Init();

	virtual void BeginPlay() override;

	// Try to fixate object to hand
	virtual bool TryToFixate();

	// Detach fixation
	virtual void TryToDetach();

	UPROPERTY()
	FString GripperName;

	// Fixated object

	UPROPERTY()
	AStaticMeshActor* FixatedObject;

	UPROPERTY()
	bool bObjectGrasped;


protected:

	UPROPERTY(EditAnywhere)
	float GraspRadius = 10.f;

	UPROPERTY()
	URStaticMeshComponent* Gripper;

	UPROPERTY()
	URStaticMeshComponent* Gripper2;

        UPROPERTY(EditAnywhere)
          UPhysicsConstraintComponent* Constraint;

        UPROPERTY(EditAnywhere)
          UPhysicsConstraintComponent* Constraint2;
	// Function called when an item enters the fixation overlap area
	UFUNCTION()
	virtual void OnFixationGraspAreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	// Function called when an item leaves the fixation overlap area
	UFUNCTION()
	virtual void OnFixationGraspAreaEndOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// Array of items currently in reach (overlapping the sphere component)
	TArray<AStaticMeshActor*> ObjectsInReach;


	// Fixate object to hand
	virtual void FixateObject(AStaticMeshActor* InSMA);

        UPROPERTY()
          bool bGraspObjectGravity;
};
