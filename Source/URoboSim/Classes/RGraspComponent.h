// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
#pragma once

#include "CoreMinimal.h"
#include "RStaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Engine/StaticMeshActor.h"
#include "RGraspComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UROBOSIM_API URGraspComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	URGraspComponent();

	virtual void Init(URStaticMeshComponent* InGripper);

	virtual void BeginPlay() override;

	// Try to fixate object to hand
	virtual bool TryToFixate();

	// Detach fixation
	virtual void TryToDetach();

	UPROPERTY(EditAnywhere, Category = "MC")
	FString GripperName;
	// Fixated object
	AStaticMeshActor* FixatedObject;
protected:


	// Object maximum length (cm)
	UPROPERTY(EditAnywhere, Category = "MC")
	float ObjectMaxLength;

	// Object maximum mass (kg)
	UPROPERTY(EditAnywhere, Category = "MC")
	float ObjectMaxMass;

	// Flag if the object should be welded to the hand
	UPROPERTY(EditAnywhere, Category = "MC")
	bool bWeldFixation;

	UPROPERTY()
	URStaticMeshComponent* Gripper;


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


	// Check if the static mesh actor can be grasped
	virtual bool CanBeGrasped(AStaticMeshActor* InActor);
};
