// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Physics/RPhysicsConstraintComponent.h"
#include "RUtilityClasses.h"
#include "RJoint.generated.h"

class ARModel;

USTRUCT()
struct FMimicJointParameter
{
  GENERATED_BODY()
  public:

    UPROPERTY(EditAnywhere)
      URJoint* MimicJoint = nullptr;

    UPROPERTY(EditAnywhere)
      float Multiplier = 0;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UROBOSIM_API URJoint : public USceneComponent
{
	GENERATED_BODY()

public:
	URJoint();

        // bool IsTickable() const override;
        virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
        // TStatId GetStatId() const override;
	// virtual UWorld* GetTickableGameObjectWorld() const override { return GetOuter()->GetWorld(); }
        virtual void BeginDestroy() override;

        UPROPERTY()
          bool bBreakEnabled;

        UPROPERTY()
          bool bActuate;

        UPROPERTY()
          bool bUseParentModelFrame = true;

        virtual void SetParentChild(URLink* Parent, URLink* Child);
        virtual void SetParentChild(AActor* InParent, AActor* InChild);

        UPROPERTY()
          FString ParentName;

        UPROPERTY()
          FString ChildName;

        UPROPERTY()
          class URLink* Parent = nullptr;

        UPROPERTY()
          class URLink* Child = nullptr;

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
  virtual void SetSimulatePhysics(bool bEnablePhysics);

  virtual void Break();

    UPROPERTY(EditAnywhere)
    URConstraintComponent* Constraint;

    virtual float GetEncoderValue();

public:

    UPROPERTY(EditAnywhere)
      FTransform Pose;

    UPROPERTY()
      FString PoseRelativeTo;

    UPROPERTY()
      float MaxJointVel = -1;

    UPROPERTY(EditAnywhere)
     bool bIsMimic = false;

    UPROPERTY(EditAnywhere)
      TArray<FMimicJointParameter> MimicJointList;

 protected:

    virtual void UpdateEncoder();
    virtual void UpdateVelocity(float InDeltaTime);

    UPROPERTY()
      float OldJointValue = 0;

    UPROPERTY()
      float JointVelocity = 0;
};
