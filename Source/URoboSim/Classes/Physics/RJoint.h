// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Andrei Haidu (http://haidu.eu)

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SDF/SDFDataAsset.h"
#include "RPhysicsConstraintComponent.h"
#include "RJoint.generated.h"

// Forward declarations
class ARModel;
class USDFJoint;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UROBOSIM_API URJoint : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URJoint();

        // UPROPERTY()
        //   FJointInformation JointInformation;

        UPROPERTY()
          bool bBreakEnabled;

        UPROPERTY()
          bool bActuate;

        UPROPERTY()
          bool bUseParentModelFrame;

        virtual void SetParentChild(URLink* Parent, URLink* Child);

        UPROPERTY()
          FString ParentName;

        UPROPERTY()
          FString ChildName;

        UPROPERTY()
          class URLink* Parent;
        // FString ChildName;

        UPROPERTY()
          class URLink* Child;
    // FString ParentName;

	virtual float GetJointPosition();
	virtual float GetJointPositionInUUnits();
	virtual float GetJointVelocity();

	virtual void SetJointPosition(float Angle);
	virtual void SetJointVelocity(float Velocity);
	virtual void SetJointVelocityInUUnits(float Velocity);
	virtual void SetJointEffort(float Effort);
	virtual void SetJointEffortFromROS(float Effort);
        virtual void EnableMotor(bool InEnable);

    static void Load(ARModel* OutModel, USDFJoint* InJoint);

    UPROPERTY(EditAnywhere)
    URConstraintComponent* Constraint;

    virtual float GetEncoderValue();
protected:
	// Called when the game starts
	// virtual void BeginPlay() override;

    // static void SetupConstraint(ARModel* OutModel, URJoint* OutOwner, USDFJoint* InJoint);




public:
    // Called every frame
    // virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    virtual void UpdateVelocity();
    // virtual void UpdateJointStates();
    virtual void UpdateEncoder();

    UPROPERTY(EditAnywhere)
      FTransform Pose;

    UPROPERTY()
      float DesiredJointPose = 0;

    UPROPERTY()
      float MaxJointVel = -1;

    UPROPERTY()
      float AccumulatatedJointMass = 0;
};
