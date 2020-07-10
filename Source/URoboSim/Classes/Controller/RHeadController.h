#pragma once

#include "Physics/RModel.h"
#include "Controller/RController.h"
#include "RHeadController.generated.h"


UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URHeadTrajectoryController : public URController
{
    GENERATED_BODY()
public:
	URHeadTrajectoryController();

	virtual void Init(ARModel* InModel) override;
	virtual void Tick(float InDeltaTime);

	UPROPERTY()
	float AngleError;

	UPROPERTY()
	FString FrameId;

	UPROPERTY()
	FVector Point;

	UPROPERTY()
	FString PointingFrame;

	UPROPERTY()
	FVector Axis;

	UPROPERTY()
	ARModel* Model;

    virtual void UpdateHeadDirection(){};
protected:
    UPROPERTY()
      URJointController* JointController;

    UPROPERTY()
      URControllerComponent* ControllerComp;

	virtual FVector CalculateNewViewDirection();
	virtual void MoveToNewPosition(FVector InNewDirection){};
    virtual void CheckPointHeadState(){};

};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URPR2HeadTrajectoryController : public URHeadTrajectoryController
{
    GENERATED_BODY()
public:
    virtual void UpdateHeadDirection();
protected:
	virtual void MoveToNewPosition(FVector InNewDirection) override;
    virtual void CheckPointHeadState();

};
