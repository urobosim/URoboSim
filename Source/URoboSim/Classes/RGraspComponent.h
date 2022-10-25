// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
#pragma once

#include "CoreMinimal.h"
#include "Physics/RStaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Components/BoxComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "RGraspComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API URGraspComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	URGraspComponent();

	virtual void OnComponentCreated() override;

	virtual void Init(UPrimitiveComponent* InGripper1, UPrimitiveComponent* InGripper2);
	virtual void Init(UPrimitiveComponent* InGripper);
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
	AActor* ObjectToPublish;

	UPROPERTY()
	bool bObjectGrasped;

	UPROPERTY(EditAnywhere)
	UPhysicsConstraintComponent* Constraint;

protected:
	// UPROPERTY()
	// URTFPublisher* TFPublisher;

	UPROPERTY(EditAnywhere)
	float GraspRadius = 5.f;

	UPROPERTY()
	UPrimitiveComponent* Gripper;

	UPROPERTY()
	UPrimitiveComponent* Gripper2;


	UPROPERTY(EditAnywhere)
	UPhysicsConstraintComponent* Constraint2;
	// Function called when an item enters the fixation overlap area
	UFUNCTION()
	virtual void OnFixationGraspAreaBeginOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                                             class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	                                             bool bFromSweep, const FHitResult& SweepResult);

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
