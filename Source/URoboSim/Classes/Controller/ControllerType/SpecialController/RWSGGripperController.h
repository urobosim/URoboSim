#pragma once

#include "Physics/RModel.h"
#include "Controller/ControllerType/JointController/RJointController.h"
#include "RGripperControllerBase.h"
// clang-format off
#include "RWSGGripperController.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URWSGGripperControllerParameter: public URGripperControllerBaseParameter
{
  GENERATED_BODY()

public:

  UPROPERTY(EditAnywhere)
  FString GripperFingerJointName;
};

UCLASS()
class UROBOSIM_API URWSGGripperController: public URGripperControllerBase
{
  GENERATED_BODY()

public:

  virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) override;
  virtual void Init() override;
  virtual void Tick(const float &InDeltaTime) override;
  virtual void SetPose(const float& InPose);

  UPROPERTY(EditAnywhere)
  FString GripperFingerJointName;

  UPROPERTY()
  URJoint *GripperFingerJoint;

 protected:

  UPROPERTY()
    float Timer = 0;

  UPROPERTY()
    float Timeout = 1;

  UPROPERTY()
    bool bTimoutActive = false;
};
