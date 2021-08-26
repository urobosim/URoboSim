
#pragma once

#include "CoreMinimal.h"
#include "Physics/RJoint.h"
#include "Factory/RJointFactory.h"
#include "SDF/SDFJointAxis.h"
#include "AJointActor.generated.h"


UCLASS( ClassGroup=Physics, meta=(BlueprintSpawnableComponent), DefaultToInstanced, ConversionRoot, MinimalAPI, ComponentWrapperClass)
class AJointActor : public AActor
{
  GENERATED_BODY()

public:
    AJointActor();

  UPROPERTY(EditInstanceOnly, Category=Constraint)
    AActor* ConstraintActor1;

  UPROPERTY(EditInstanceOnly, Category=Constraint)
    AActor* ConstraintActor2;

  UPROPERTY(EditAnywhere, Category=Constraint)
  USDFJointAxis* Axis;

  // Called every frame
  virtual void Tick(float DeltaTime) override;
  virtual void BeginPlay() override;
  virtual bool CreateJoint();

  UPROPERTY(VisibleAnywhere)
    float CurrentJointPos;

  UPROPERTY(Category = ConstraintActor, VisibleAnywhere, BlueprintReadOnly, meta = (ExposeFunctionCategories = "JointDrive,Physics|Components|PhysicsConstraint", AllowPrivateAccess = "true"))
    URJoint* Joint;
protected:


  UPROPERTY()
    USDFJoint* JointDescription;

  UPROPERTY(EditAnywhere, Instanced)
    URJointBuilder* JointType;

#if WITH_EDITOR
	virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif // WITH_EDITOR
};
