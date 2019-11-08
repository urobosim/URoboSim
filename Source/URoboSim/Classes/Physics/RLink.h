// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "CoreMinimal.h"
// #include "Components/SceneComponent.h"
#include "SDF/SDFDataAsset.h"
#include "RStaticMeshComponent.h"
#include "RLink.generated.h"

class ARModel;
class USDFLink;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UROBOSIM_API URLink : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URLink();


protected:
	// Called when the game starts
	// virtual void BeginPlay() override;

    static void SetVisuals(URLink* OutLink, TArray<USDFVisual*> InVisuals);
    static void SetVisual(URLink* OutLink, USDFVisual* InVisual);
    static void SetCollisions(URLink* OutLink, TArray<USDFCollision*> InCollisions);
    static void SetCollision(URLink* OutLink, USDFCollision* InCollision);
    static void SetVirtualCollision(URLink* OutLink, USDFLink* InLink);
    static void SetInertial(URLink* OutLink, USDFLinkInertial* InInertial);
	// static void SetMass(URLink* OutLink, USDF);
    static void SetCollisionProfile(URLink* OutLink, bool InSelfColide);
    static void SetSimulateGravity(URLink* OutLink, bool InUseGravity);
    static bool CreateCollisionForMesh(UStaticMesh* OutLink, ESDFGeometryType Type);


	UPROPERTY()
	TArray<class URJoint*> Joints;

public:

	UPROPERTY()
	ARModel* Model;

	UPROPERTY(EditAnywhere)
	TArray<class URStaticMeshComponent*> Visuals;
	UPROPERTY(EditAnywhere)
	TArray<class URStaticMeshComponent*> Collisions;

	virtual void SetPose(FTransform InPose);
	virtual void SetPose(FVector InLocation, FQuat InRotation);

	// UPROPERTY()
	// FString ChildFrame;

	// UPROPERTY()
	// FString ParentFrame;

	URStaticMeshComponent* GetVisual();
	URStaticMeshComponent* GetCollision();
        TArray<class URJoint*> GetJoints();
	// virtual FString GetParentFrame(FString InDefaultFrame);

	float GetNumCollisions();
	// Load link from sdf data
	static void Load(ARModel* OutModel, USDFLink* InLink);
	void AddJoint(class URJoint* InJoint);

	virtual void UpdateVelocity();
	virtual void UpdateEncoder();
	// virtual void UpdateJointStates();
	virtual void SetNextVelocities();

	UPROPERTY(EditAnywhere)
	FTransform Pose;

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
