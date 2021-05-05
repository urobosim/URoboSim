#pragma once

#include "Controller/RControllerComponent.h"
#include "Physics/RModel.h"
#include "RJointController.h"
// clang-format off
#include "RHeadController.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URHeadTrajectoryController : public URController
{
	GENERATED_BODY()

public:
	URHeadTrajectoryController();

public:
	virtual void Init() override;

	virtual void Tick(const float &InDeltaTime);

public:
	virtual void UpdateHeadDirection(){};

public:
	float AngleError;

	FString FrameId;

	FVector Point;

	FString PointingFrame;

	FVector Axis;

protected:
	virtual FVector CalculateNewViewDirection();

	virtual void MoveToNewPosition(FVector InNewDirection){};

	virtual void CheckPointHeadState(){};

protected:
	float ActionStartTime;

	URJointController *JointController;
};

UCLASS()
class UROBOSIM_API URPR2HeadTrajectoryController : public URHeadTrajectoryController
{
	GENERATED_BODY()

public:
	virtual void UpdateHeadDirection();

protected:
	virtual void MoveToNewPosition(FVector InNewDirection) override;

	virtual void CheckPointHeadState();
};
