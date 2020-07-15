#pragma once

#include "Controller/RController.h"
#include "Controller/RJointController.h"
#include "Physics/RModel.h"
#include "RGraspComponent.h"
#include "ConstructorHelpers.h"
#include "Physics/RJoint.h"
#include "RGripperController.generated.h"

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

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URGripperController : public URController
{
  GENERATED_BODY()
    public:
    URGripperController();

  virtual void Init(ARModel* InModel) override;
  virtual bool Grasp();
  virtual void Release();
  virtual void UpdateGripper();

  virtual void CheckGripperActionResult(float InError, float InThreshold);
  virtual void Tick(float InDeltaTime);

  UPROPERTY()
    URGraspComponent* GraspComponent;

  UPROPERTY(EditAnywhere)
    FGraspComponentSetting GraspCompSetting;

  UPROPERTY()
    float GripperPosition = 0;

  UPROPERTY()
    float Position;

  UPROPERTY()
    float OldPosition;

  UPROPERTY()
    float MaxEffort;

  UPROPERTY()
    bool bStalled;


  UPROPERTY()
    FGraspResult Result;

  UPROPERTY(EditAnywhere)
    FString RightJointName;

  UPROPERTY(EditAnywhere)
    FString LeftJointName;

  UPROPERTY(EditAnywhere)
    FString RightFingerTipName;

  UPROPERTY(EditAnywhere)
    FString LeftFingerTipName;

  UPROPERTY()
    URJoint* RightFinger ;

  UPROPERTY()
    URJoint* LeftFinger ;

  UPROPERTY()
    URJoint* RightFingerTip;

  UPROPERTY()
    URJoint* LeftFingerTip;


 protected:

  UPROPERTY()
    bool bSuccessGrasp = false;

  UPROPERTY()
    bool bMoved = false;

  UPROPERTY()
    ARModel* Model;

  UPROPERTY()
    URControllerComponent* ControllerComp;

  UPROPERTY()
    URJointController* JointController;

  UPROPERTY(EditAnywhere)
    FString GraspComponentName;

  UPROPERTY()
    float JointValue;
};
