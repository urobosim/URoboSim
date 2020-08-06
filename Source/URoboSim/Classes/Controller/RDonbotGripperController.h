#pragma once

#include "ConstructorHelpers.h"
#include "Controller/RController.h"
#include "Controller/RJointController.h"
#include "Physics/RJoint.h"
#include "Physics/RModel.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "RGraspComponent.h"
#include "RGripperController.h"
// clang-format off
#include "RDonbotGripperController.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URDonbotGripperController : public URController
{
  GENERATED_BODY()
public:
  URDonbotGripperController();

  virtual void Init(ARModel* InModel) override;
  virtual void Grasp();
  virtual void Release();
  virtual void UpdateGripper();
  virtual void Tick(float InDeltaTime) override;

protected:
  UPROPERTY()
  URGraspComponent* GraspComponent;

  UPROPERTY(EditAnywhere)
  FGraspComponentSetting GraspCompSetting;

  UPROPERTY()
  bool bIsGrasping = false;

  UPROPERTY(EditAnywhere)
  bool bAutoGrasp;

  UPROPERTY(EditAnywhere)
  FString FingerRightName;

  UPROPERTY(EditAnywhere)
  FString FingerLeftName;

  UPROPERTY()
  URJoint* FingerRight = nullptr;

  UPROPERTY()
  URJoint* FingerLeft = nullptr;

  UPROPERTY(EditAnywhere)
  float GripperPosition;

  UPROPERTY()
  ARModel* Model = nullptr;

  UPROPERTY()
  URJointController* JointController = nullptr;

  UPROPERTY(EditAnywhere)
  FString GraspComponentName;

  UPROPERTY()
  UPhysicsHandleComponent* PhysicsHandle = nullptr;
};
