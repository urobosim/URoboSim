// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once


#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "RStaticMeshComponent.h"
#include "SDF/SDFJoint.h"
#include "Sensor/REncoder.h"
#include "Physics/RLink.h"
#include "RPhysicsConstraintComponent.generated.h"

USTRUCT()
struct FJointInformation
{
  GENERATED_BODY()
  public:


  UPROPERTY()
  URLink* Parent;

  UPROPERTY()
  URLink* Child;

  UPROPERTY()
  FVector RefAxis;

  UPROPERTY()
  float Upper;

  UPROPERTY()
  float Lower;

  UPROPERTY()
  FQuat QInitial;
};

UCLASS()
class UROBOSIM_API URConstraintComponent : public UPhysicsConstraintComponent
{
  GENERATED_BODY()
    public:

    // UPROPERTY()
    // FJointInformation JointInformation;

  UPROPERTY()
    FQuat QInitial;

  UPROPERTY(EditInstanceOnly)
    float Upper;

  UPROPERTY(EditInstanceOnly)
    float Lower;

  UPROPERTY(EditInstanceOnly)
    float SoftUpper = 0;

  UPROPERTY(EditInstanceOnly)
    float SoftLower = 0;

  virtual void ConnectToComponents(){};
  virtual void EnableMotor(bool InEnable){};
  virtual void SetTargetPosition(float InTargetPos){};
  virtual float GetConstraintPosition(){return 0;};
  virtual void SetPosition(USDFJoint* InJoint){};
  virtual void SetParentChild(UStaticMeshComponent* InParent, UStaticMeshComponent* InChild);


  virtual float ClampJointStateToConstraintLimit(float InJointState){return InJointState;};
  virtual float GetJointPosition(){return 0.;};
  virtual float GetJointVelocity(){return 0.;};

  virtual float GetJointPositionInUUnits(){return 0.;};
  virtual float GetJointVelocityInUUnits(){return 0.;};

  virtual void SetJointPosition(float Angle, FHitResult * OutSweepHitResult){};
  virtual void SetJointVelocity(float Velocity){UE_LOG(LogTemp, Warning, TEXT("test "));};
  virtual void SetJointVelocityInUUnits(float Velocity){UE_LOG(LogTemp, Warning, TEXT("test "));};
  virtual void SetJointEffort(float Effort){};
  virtual void SetJointEffortFromROS(float InEffort){};

  virtual void UpdateJointVelocity(float InDeltaT = 0){};
  virtual void UpdateEncoderValue(float InValue);
  virtual float CheckPositionRange(float InTargetJointPos){return InTargetJointPos;};

  virtual float GetUpperLimit();
  virtual float GetLowerLimit();

  UPROPERTY()
    float JointAccuracy;

  UPROPERTY()
    float Limit;

  UPROPERTY()
    UREncoder* Encoder;

  UPROPERTY(EditInstanceOnly)
    FVector RefAxis;

  UPROPERTY()
    float RotationOffset;

  UPROPERTY()
    FVector Offset;
 protected:


  UPROPERTY()
    float TargetVelocity;

  UPROPERTY()
    UStaticMeshComponent* Parent;

  UPROPERTY()
    UStaticMeshComponent* Child;

};

UCLASS(ClassGroup=Physics, meta=(BlueprintSpawnableComponent), HideCategories=(Activation,"Components|Activation", Physics, Mobility), ShowCategories=("Physics|Components|PhysicsConstraint"))
class UROBOSIM_API URFixedConstraintComponent : public URConstraintComponent
{
  GENERATED_BODY()
    public:
    virtual void ConnectToComponents();
  virtual void SetPosition(USDFJoint *InJoint);

};

UCLASS(ClassGroup=Physics, meta=(BlueprintSpawnableComponent), HideCategories=(Activation,"Components|Activation", Physics, Mobility), ShowCategories=("Physics|Components|PhysicsConstraint"))
class UROBOSIM_API URPrismaticConstraintComponent : public URFixedConstraintComponent
{
  GENERATED_BODY()
    public:

    URPrismaticConstraintComponent(){
    Encoder = CreateDefaultSubobject<URLinearEncoder>(FName(*(GetName() + TEXT("_Encoder"))));
  };


  virtual float ClampJointStateToConstraintLimit(float InJointState) override;
  virtual void SetPosition(USDFJoint* InJoint);
  virtual void BeginPlay() override;

  virtual float GetJointPosition() override;
  virtual float GetJointVelocity() override;

  virtual float GetJointPositionInUUnits() override;
  virtual float GetJointVelocityInUUnits() override;

  virtual void SetJointPosition(float Angle, FHitResult * OutSweepHitResult);
  virtual void SetJointVelocity(float Velocity);
  virtual void SetJointVelocityInUUnits(float Velocity);
  virtual void SetJointEffort(float Effort);
  virtual void SetJointEffortFromROS(float InEffort);
  virtual void EnableMotor(bool InEnable);
  virtual void SetTargetPosition(float InTargetPos);



 protected:
  virtual void UpdateJointVelocity(float InDeltaT = 0);
  virtual void ConnectToComponents() override;
 private:
  UPROPERTY()
    FVector ParentChildDistance;

};


UCLASS(ClassGroup=Physics, meta=(BlueprintSpawnableComponent), HideCategories=(Activation,"Components|Activation", Physics, Mobility), ShowCategories=("Physics|Components|PhysicsConstraint"))
class UROBOSIM_API URContinuousConstraintComponent : public URFixedConstraintComponent
{
  GENERATED_BODY()
    public:
    URContinuousConstraintComponent(){
    Encoder = CreateDefaultSubobject<URAngularEncoder>(FName(*(GetName() + TEXT("_Encoder"))));
  };

  virtual void SetTargetPosition(float InTargetPos);
  virtual void EnableMotor(bool InEnable);
  virtual float GetConstraintPosition() override;
  virtual float GetJointPosition() override;
  virtual float GetJointVelocity() override;
  virtual float GetJointPositionInUUnits() override;
  virtual float GetJointVelocityInUUnits() override;

  virtual float CheckPositionRange(float InTargetJointPos);

  virtual void SetJointPosition(float Angle, FHitResult * OutSweepHitResult);
  virtual void SetJointVelocity(float Velocity);
  virtual void SetJointVelocityInUUnits(float Velocity);
  virtual void SetJointEffort(float Effort);
  virtual void SetJointEffortFromROS(float InEffort);

  virtual void BeginPlay() override;


  virtual void UpdateJointVelocity(float InDeltaT = 0);
};

UCLASS(ClassGroup=Physics, meta=(BlueprintSpawnableComponent), HideCategories=(Activation,"Components|Activation", Physics, Mobility), ShowCategories=("Physics|Components|PhysicsConstraint"))
class UROBOSIM_API URScrewConstraintComponent : public URContinuousConstraintComponent
{
  GENERATED_BODY()
    };

UCLASS(ClassGroup=Physics, meta=(BlueprintSpawnableComponent), HideCategories=(Activation,"Components|Activation", Physics, Mobility), ShowCategories=("Physics|Components|PhysicsConstraint"))
class UROBOSIM_API URRevoluteConstraintComponent : public URContinuousConstraintComponent
{
  GENERATED_BODY()
    public:
    virtual void SetTargetPosition(float InTargetPos);

  virtual float ClampJointStateToConstraintLimit(float InJointState) override;

};
