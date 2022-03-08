// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "Physics/RStaticMeshComponent.h"
#include "RLink.generated.h"

class ARModel;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UROBOSIM_API URLink : public UObject
{
	GENERATED_BODY()

public:
	URLink();


protected:

	UPROPERTY()
	TArray<class URJoint*> Joints;

public:

	UPROPERTY()
	ARModel* Model;

	UPROPERTY(EditAnywhere)
	TArray<class UStaticMeshComponent*> Visuals;
	UPROPERTY(EditAnywhere)
	TArray<class UStaticMeshComponent*> Collisions;

	virtual void SetPoseComponent(USceneComponent *&InPoseComponent) { PoseComponent = InPoseComponent; }
	virtual const FTransform GetPose() const { return PoseComponent->GetComponentTransform(); }

	virtual void DisableCollision();
	virtual void EnableCollision();

	UStaticMeshComponent* GetVisual();
	UStaticMeshComponent* GetCollision();
	UStaticMeshComponent* GetCollision(FString InCollisionName, bool bExactMatch = false);
        TArray<class URJoint*> GetJoints();

	float GetNumCollisions();

	void AddJoint(class URJoint* InJoint);


	UPROPERTY(VisibleAnywhere)
	USceneComponent *PoseComponent;

        UPROPERTY()
        bool bAttachedToParent = false;
};

USTRUCT()
struct FLinkInformation
{
  GENERATED_BODY()
  public:

  TArray<URLink*> Childs;
};
