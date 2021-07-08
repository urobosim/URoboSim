// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Physics/RPhysicsConstraintComponent.h"
#include "RUtilityClasses.h"
#include "RJoint.generated.h"

class ARModel;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UROBOSIM_API URJoint : public UObject
{
	GENERATED_BODY()

public:
	URJoint();

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

        UPROPERTY()
          class URLink* Child;

  FJointState GetJointState();
	virtual float GetJointPosition();
	virtual float GetJointPositionInUUnits();
	virtual float GetJointVelocity();
        virtual float ClampJointStateToConstraintLimit(float InJointState){return Constraint->ClampJointStateToConstraintLimit(InJointState);};

  void SetMotorJointState(const FJointState &JointState);
	virtual void SetJointPosition(float Angle, FHitResult * OutSweepHitResult);
	virtual void SetJointVelocity(float Velocity);
	virtual void SetJointVelocityInUUnits(float Velocity);
	virtual void SetJointEffort(float Effort);
	virtual void SetJointEffortFromROS(float Effort);
  virtual void SetDrive(const FEnableDrive &EnableDrive);

    UPROPERTY(EditAnywhere)
    URConstraintComponent* Constraint;

    virtual float GetEncoderValue();

public:

    virtual void UpdateVelocity(float InDeltaTime);
    virtual void UpdateEncoder();

    UPROPERTY(EditAnywhere)
      FTransform Pose;

    UPROPERTY()
      float MaxJointVel = -1;

    UPROPERTY()
      float AccumulatatedJointMass = 0;
};
