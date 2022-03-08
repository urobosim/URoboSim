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
class UROBOSIM_API URJoint : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	URJoint();

        bool IsTickable() const override;
        void Tick(float DeltaTime) override;
        TStatId GetStatId() const override;
	virtual UWorld* GetTickableGameObjectWorld() const override { return GetOuter()->GetWorld(); }
        virtual void BeginDestroy() override;

        UPROPERTY()
          bool bBreakEnabled;

        UPROPERTY()
          bool bActuate;

        UPROPERTY()
          bool bUseParentModelFrame;

        virtual void SetParentChild(URLink* Parent, URLink* Child);
        virtual void SetParentChild(AActor* InParent, AActor* InChild);

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

    UPROPERTY(EditAnywhere)
      FTransform Pose;

    UPROPERTY()
      float MaxJointVel = -1;

    UPROPERTY(EditAnywhere)
     bool bHasMimic = false;

    UPROPERTY(EditAnywhere)
      TArray<FMimicJointParameter> MimicJointList;

 protected:

    virtual void UpdateEncoder();
};
