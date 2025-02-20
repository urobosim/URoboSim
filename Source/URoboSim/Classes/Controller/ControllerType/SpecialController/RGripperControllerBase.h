#pragma once

#include "Physics/RModel.h"
#include "Controller/ControllerType/JointController/RJointController.h"
// clang-format off
#include "RGripperControllerBase.generated.h"
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

  UPROPERTY(EditAnywhere)
  float Radius = 3.0;
};

UCLASS()
class UROBOSIM_API URGripperControllerBaseParameter : public URControllerParameter
{
  GENERATED_BODY()
public:
    //tracebot_right_gripper_joint_intermediate_1
  UPROPERTY(EditAnywhere)
  TArray<FString> GripperJointNames;

  UPROPERTY(EditAnywhere)
  FString GraspComponentName;

  UPROPERTY(EditAnywhere)
  FGraspComponentSetting GraspCompSetting;

  UPROPERTY(EditAnywhere)
  UJointControllerMode Mode;

  UPROPERTY(EditAnywhere)
  FEnableDrive EnableDrive;

  UPROPERTY(EditAnywhere)
    bool bOverwriteConfig = true;

  UPROPERTY(EditAnywhere)
    bool bInvertGraspCondition = false;
};

UCLASS()
class UROBOSIM_API URGripperControllerBase : public URController
{
  GENERATED_BODY()

public:
  URGripperControllerBase();

public:
  virtual void Init() override;

  virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) override;

  virtual bool Grasp();

  virtual void Release();

  virtual void Tick(const float &InDeltaTime);

public:
  URGraspComponent *GraspComponent;

  UPROPERTY(EditAnywhere)
  FGraspComponentSetting GraspCompSetting;

  UPROPERTY(EditAnywhere)
  URLink* ReferenceLink;

  TMap<FString,float> OldPositions;

  UPROPERTY(EditAnywhere)
    TArray<FString> GripperJointNames;

  UPROPERTY()
  TArray<URJoint*> GripperJoints;

  UPROPERTY(EditAnywhere)
    bool bInvertGraspCondition = false;

  UPROPERTY(EditAnywhere)
  UJointControllerMode Mode;

  UPROPERTY(EditAnywhere)
    bool bOverwriteConfig = true;

  UPROPERTY(EditAnywhere)
  FEnableDrive EnableDrive;

protected:
  URJointController *JointController;

  UPROPERTY(EditAnywhere)
  FString GraspComponentName;

  UPROPERTY()
    bool bDebugMode = false;

  int GraspInversion = 1;

};
