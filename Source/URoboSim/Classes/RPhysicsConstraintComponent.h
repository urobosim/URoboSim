// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once


#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "RStaticMeshComponent.h"
#include "SDF/SDFJoint.h"
#include "Sensor/REncoder.h"
/* #include "PIDController3D.h" */
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

    UPROPERTY()
    FJointInformation JointInformation;

  UPROPERTY()
    FQuat QInitial;

  typedef void (URConstraintComponent::*UpdateStateFunctionPtr)();

  UpdateStateFunctionPtr UpdateFunctionPtr;

  UPROPERTY()
    float Upper;

  UPROPERTY()
    float Lower;
  // virtual void Init(USDFJoint* InJoint, UActorComponent* Parent, UActorComponent* Child){};
  virtual void ConnectToComponents(){};
  virtual void EnableMotor(bool InEnable){};
  virtual void SetTargetPosition(float InTargetPos){};
  virtual float GetConstraintPosition(){return 0;};
  virtual void SetPosition(USDFJoint* InJoint){};
  virtual void SetParentChild(URStaticMeshComponent* InParent, URStaticMeshComponent* InChild);


  virtual float GetJointPosition(){return 0.;};
  virtual float GetJointVelocity(){return 0.;};

  virtual float GetJointPositionInUUnits(){return 0.;};
  virtual float GetJointVelocityInUUnits(){return 0.;};

  virtual void SetJointPosition(float Angle){};
  virtual void SetJointVelocity(float Velocity){UE_LOG(LogTemp, Warning, TEXT("test "));};
  virtual void SetJointVelocityInUUnits(float Velocity){UE_LOG(LogTemp, Warning, TEXT("test "));};
  virtual void SetJointEffort(float Effort){};
  virtual void SetJointEffortFromROS(float InEffort){};

  virtual void UpdateJointVelocity(){};
  virtual void UpdateEncoderValue(float InValue);
  virtual float CheckPositionRange(float InTargetJointPos){return InTargetJointPos;};

  UPROPERTY(EditAnywhere)
    float P;

  UPROPERTY(EditAnywhere)
    float I;

  UPROPERTY(EditAnywhere)
    float D;

  UPROPERTY()
    float JointAccuracy;

  UPROPERTY()
    float Limit;

  UPROPERTY()
    UREncoder* Encoder;

  UPROPERTY(VisibleAnywhere)
    FVector RefAxis;

  UPROPERTY()
    float RotationOffset;

  UPROPERTY()
    FVector Offset;
 protected:


  // UPROPERTY()
  // FPIDController3D PID;

  UPROPERTY()
    float TargetVelocity;

  UPROPERTY()
    URStaticMeshComponent* Parent;

  UPROPERTY()
    URStaticMeshComponent* Child;

  bool bBreakEnabled;
};

UCLASS()
class UROBOSIM_API URFixedConstraintComponent : public URConstraintComponent
{
    GENERATED_BODY()
public:
	// virtual void Init(USDFJoint* InJoint, UActorComponent* Parent, UActorComponent* Child);
	virtual void ConnectToComponents();
	virtual void SetPosition(USDFJoint *InJoint);

};

UCLASS()
class UROBOSIM_API URPrismaticConstraintComponent : public URFixedConstraintComponent
{
  GENERATED_BODY()
    public:

    URPrismaticConstraintComponent(){
    Encoder = CreateDefaultSubobject<URLinearEncoder>(FName(*(GetName() + TEXT("_Encoder"))));
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;
    P = 1;
    I = 1;
  };


  virtual void SetPosition(USDFJoint* InJoint);
  virtual void BeginPlay() override;

  virtual float GetJointPosition() override;
  virtual float GetJointVelocity() override;

  virtual float GetJointPositionInUUnits() override;
  virtual float GetJointVelocityInUUnits() override;

  virtual void SetJointPosition(float Angle);
  virtual void SetJointVelocity(float Velocity);
  virtual void SetJointVelocityInUUnits(float Velocity);
  virtual void SetJointEffort(float Effort);
  virtual void SetJointEffortFromROS(float InEffort);
  virtual void EnableMotor(bool InEnable);
  virtual void SetTargetPosition(float InTargetPos);



 protected:
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
  virtual void UpdateJointVelocity();
  virtual void ConnectToComponents() override;
 private:
  UPROPERTY()
    FVector ParentChildDistance;

};


UCLASS()
class UROBOSIM_API URContinuousConstraintComponent : public URFixedConstraintComponent
{
    GENERATED_BODY()
public:
      URContinuousConstraintComponent(){
      Encoder = CreateDefaultSubobject<URAngularEncoder>(FName(*(GetName() + TEXT("_Encoder"))));
      PrimaryComponentTick.bCanEverTick = true;
      PrimaryComponentTick.TickGroup = TG_PrePhysics;
      P = 1;
      I = 1;
      // D = 0.01;
    };

        virtual void SetTargetPosition(float InTargetPos);
	virtual void EnableMotor(bool InEnable);
        virtual float GetConstraintPosition() override;
    virtual float GetJointPosition() override;
    virtual float GetJointVelocity() override;
    virtual float GetJointPositionInUUnits() override;
    virtual float GetJointVelocityInUUnits() override;

	virtual float CheckPositionRange(float InTargetJointPos);

    virtual void SetJointPosition(float Angle);
    virtual void SetJointVelocity(float Velocity);
    virtual void SetJointVelocityInUUnits(float Velocity);
    virtual void SetJointEffort(float Effort);
    virtual void SetJointEffortFromROS(float InEffort);

	virtual void BeginPlay() override;


	virtual void UpdateJointVelocity();
protected:



	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};

UCLASS()
class UROBOSIM_API URScrewConstraintComponent : public URContinuousConstraintComponent
{
    GENERATED_BODY()
};

UCLASS()
class UROBOSIM_API URRevoluteConstraintComponent : public URContinuousConstraintComponent
{
    GENERATED_BODY()
public:
        virtual void SetTargetPosition(float InTargetPos);


};
