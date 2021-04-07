// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "Components/StaticMeshComponent.h"
// clang-format off
#include "RLink.generated.h"
// clang-format on

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URLink : public UObject
{
	GENERATED_BODY()

public:
	URLink();

public:
	virtual void Init();

	virtual const FTransform GetPose() const { return PoseComponent->GetComponentTransform(); }

	virtual void SetPose(const FTransform &Pose);

	virtual const USceneComponent *GetPoseComponent() const { return PoseComponent; }

	virtual const TArray<UStaticMeshComponent *> GetVisualMeshes() const { return VisualMeshes; }

	virtual void AddVisualMesh(UStaticMeshComponent *&VisualMesh) { VisualMeshes.Add(VisualMesh); }

	virtual const TArray<UStaticMeshComponent *> GetCollisionMeshes() const { return CollisionMeshes; }

	virtual void AddCollisionMesh(UStaticMeshComponent *&CollisionMesh) { CollisionMeshes.Add(CollisionMesh); }

	virtual void AttachToComponent(USceneComponent *Parent);

	virtual void SetEnableGravity(const bool &bGravityEnabled);

	virtual void SetSimulatePhysics(const bool &bSimulate);

	virtual void DisableCollision();

	virtual void EnableCollision();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent *PoseComponent;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMeshComponent *> VisualMeshes;

	UPROPERTY(EditAnywhere)
	TArray<UStaticMeshComponent *> CollisionMeshes;
};