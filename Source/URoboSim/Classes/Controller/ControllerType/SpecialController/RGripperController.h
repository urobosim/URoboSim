#pragma once

#include "Physics/RModel.h"
#include "Controller/ControllerType/JointController/RJointController.h"
// clang-format off
#include "RGripperController.generated.h"
// clang-format on

USTRUCT()
struct FGraspComponentSetting
{
  GENERATED_BODY()
public:
  UPROPERTY(EditAnywhere)
  FString GripperName;

  UPROPERTY(EditAnywhere)
  FVector ToolCenterPoint = FVector(15.0f, 0.0f, 0.0f);
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
class UROBOSIM_API URGripperControllerParameter : public URControllerParameter
{
  GENERATED_BODY()

public:


  UPROPERTY(EditAnywhere)
  FGraspComponentSetting GraspCompSetting;

  UPROPERTY(EditAnywhere)
  FString GripperJointName;

  UPROPERTY(EditAnywhere)
  FEnableDrive EnableDrive;

};

UCLASS()
class UROBOSIM_API URGripperController : public URController
{
  GENERATED_BODY()

public:
  URGripperController();

public:
  virtual void Init() override;

  virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) override;

  virtual bool Grasp();

  virtual void Release();

  virtual void UpdateGripper();

  virtual void CheckGripperActionResult(float InError, float InThreshold);

  virtual void Tick(const float &InDeltaTime);

public:
  URGraspComponent *GraspComponent;

  UPROPERTY(EditAnywhere)
  FGraspComponentSetting GraspCompSetting;

  float GripperPosition = 0;

  float Position;

  float OldPosition;

  float MaxEffort;

  bool bStalled;

  float PoseOffsetFromJoints = 0;

  FGraspResult Result;

  UPROPERTY(EditAnywhere)
  FString GripperJointName;

  URJoint *GripperJoint;

  UPROPERTY(EditAnywhere)
  FEnableDrive EnableDrive;

protected:
  bool bSuccessGrasp = false;

  bool bMoved = false;

  URJointController *JointController;

  UPROPERTY(EditAnywhere)
  FString GraspComponentName;

  float JointValue;
};
