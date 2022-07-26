#pragma once

#include "Physics/RModel.h"
#include "Controller/ControllerType/JointController/RJointController.h"
#include "RGripperControllerBase.h"
// clang-format off
#include "PR2GripperController.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URGripperControllerParameter : public URGripperControllerBaseParameter
{
  GENERATED_BODY()

public:
};

USTRUCT()
struct FGraspResult
{
  GENERATED_BODY()
public:
  float Position;
  float Effort;
  bool bStalled;
  bool bReachedGoal;

  void FillValues(float InPosition, float InEffort, bool InbStalled, bool InbReachedGoal)
  {
    Position = InPosition;
    Effort = InEffort;
    bStalled = InbStalled;
    bReachedGoal = InbReachedGoal;
  };
};

UCLASS()
class UROBOSIM_API UPR2GripperController : public URGripperControllerBase
{
  GENERATED_BODY()

public:
  UPR2GripperController();

public:
  virtual void Init() override;

  virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) override;

  virtual void UpdateGripper();

  virtual void CheckGripperActionResult(float InError, float InThreshold);

  virtual void Tick(const float &InDeltaTime);

public:
  float GripperPosition = 0;

  float Position;

  float MaxEffort;

  bool bStalled;

  float PoseOffsetFromJoints = 0;

  FGraspResult Result;

  // UPROPERTY(EditAnywhere)
  // FString GripperJointName2;

  // UPROPERTY()
  // URJoint *GripperJoint2;
  
  // UPROPERTY(EditAnywhere)
  // FString GripperJointName3;

  // UPROPERTY()
  // URJoint *GripperJoint3;
protected:
  bool bSuccessGrasp = false;

  bool bMoved = false;

  float JointValue;
};
