#pragma once

#include "Controller/RControllerParameter.h"
#include "Physics/RModel.h"
// clang-format off
#include "RJointControllerParameter.generated.h"
// clang-format on

UENUM()
enum class UJointControllerMode : uint8
{
  Dynamic,
  Kinematic
};

UCLASS()
class UROBOSIM_API URJointControllerParameter : public URControllerParameter
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  UJointControllerMode Mode = UJointControllerMode::Dynamic;

  UPROPERTY(EditAnywhere)
  FEnableDrive EnableDrive;

  UPROPERTY(EditAnywhere)
  bool bDisableCollision;
};