#pragma once

#include "Controller/RController.h"
#include "RUtilityClasses.h"
// clang-format off
#include "RJointController.generated.h"
// clang-format on

UENUM()
enum class UJointControllerMode : uint8
{
  Dynamic,
  Kinematic
};

USTRUCT()
struct FConfigOverwrite
{
  GENERATED_BODY()
  public:


  FConfigOverwrite();
  FConfigOverwrite(const UJointControllerMode& InMode, const FEnableDrive& InEnableDrive);

  UJointControllerMode Mode;

  UPROPERTY(EditAnywhere)
  FEnableDrive EnableDrive;
};

UCLASS()
class UROBOSIM_API URJointControllerParameter : public URControllerParameter
{
  GENERATED_BODY()

public:
  URJointControllerParameter()
  {
    Mode = UJointControllerMode::Dynamic;
    bDisableCollision = false;
    bControllAllJoints = false;
    EnableDrive.PositionStrength = 1E5;
    EnableDrive.VelocityStrength = 1E4;
    EnableDrive.MaxForce = 1E10;
  }

public:
  UPROPERTY(EditAnywhere)
  UJointControllerMode Mode;

  UPROPERTY(EditAnywhere)
  FEnableDrive EnableDrive;

  UPROPERTY(EditAnywhere)
  bool bDisableCollision;

  UPROPERTY(EditAnywhere)
  bool bControllAllJoints;
};

UCLASS()
class UROBOSIM_API URJointController : public URController
{
  GENERATED_BODY()

public:
  URJointController();

public:
  virtual void Init() override;

  virtual void Tick(const float &InDeltaTime) override;

  virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) override;

  void SetJointNames(const TArray<FString> &JointNames);
  void SetJointNames(const TArray<FString> &JointNames, const FEnableDrive &InEnableDrive);

  void AddConfigOverwrite(const FString& InJointName, const FConfigOverwrite& InConfigOverwrite);

  const UJointControllerMode GetMode();

public:
  UPROPERTY(EditAnywhere)
  TMap<FString, FJointState> DesiredJointStates;

  UPROPERTY(EditAnywhere)
  TMap<FString, FConfigOverwrite> ConfigOverwrite;

protected:
  UPROPERTY(EditAnywhere)
  FEnableDrive EnableDrive;

  UPROPERTY()
  FEnableDrive EnableDriveInternal;

  UPROPERTY(EditAnywhere)
  bool bDisableCollision;

  UPROPERTY(EditAnywhere)
  bool bControllAllJoints;

protected:
  void SetMode();
  void SetPhysics();

  void SetTargetJointState();

protected:
  UPROPERTY(EditAnywhere)
  UJointControllerMode Mode;

  UPROPERTY(EditAnywhere)
    float ErrorTolerance = 0.1;

  UPROPERTY()
  bool bEnablePhysics = true;
protected:
  void MoveJoints(const float &InDeltaTime);

  virtual void MoveJointDynamic(const float &InDeltaTime, const FString &InJointName);

  virtual void MoveJointKinematic(const FString &InJointName);

  float CalculateJointVelocity(float InDeltaTime, FString InJointName);
};
