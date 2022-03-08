// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "RBaseController.h"
// clang-format off
#include "RMIRWheelController.generated.h"
// clang-format on

USTRUCT()
struct FMIRWheelSetting
{
  GENERATED_BODY()
public:
  UPROPERTY(EditAnywhere)
	float WheelRadius = 5.f;

	UPROPERTY(EditAnywhere)
	float WheelDistanceLength = 62.f;

	UPROPERTY(EditAnywhere)
	float WheelDistanceWidth = 41.f;

	UPROPERTY(EditAnywhere)
	FString WheelLeft = FString(TEXT("left_wheel_link"));

	UPROPERTY(EditAnywhere)
	FString WheelRight = FString(TEXT("right_wheel_link"));

	UPROPERTY(EditAnywhere)
	FString WheelFrontLeftRotation = FString(TEXT("fl_caster_rotation_link"));

	UPROPERTY(EditAnywhere)
	FString WheelFrontLeftTranslation = FString(TEXT("fl_caster_wheel_link"));

	UPROPERTY(EditAnywhere)
	FString WheelFrontRightRotation = FString(TEXT("fr_caster_rotation_link"));

	UPROPERTY(EditAnywhere)
	FString WheelFrontRightTranslation = FString(TEXT("fr_caster_wheel_link"));

	UPROPERTY(EditAnywhere)
	FString WheelBackLeftRotation = FString(TEXT("bl_caster_rotation_link"));

	UPROPERTY(EditAnywhere)
	FString WheelBackLeftTranslation = FString(TEXT("bl_caster_wheel_link"));

	UPROPERTY(EditAnywhere)
	FString WheelBackRightRotation = FString(TEXT("br_caster_rotation_link"));

	UPROPERTY(EditAnywhere)
	FString WheelBackRightTranslation = FString(TEXT("br_caster_wheel_link"));

  TArray<double> WheelVelocities;
};

UCLASS()
class UROBOSIM_API URMIRWheelControllerParameter : public URBaseControllerParameter
{
  GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FMIRWheelSetting WheelSetting;
};

UCLASS()
class UROBOSIM_API URMIRWheelController : public URBaseController
{
  GENERATED_BODY()

public:
  URMIRWheelController();

  virtual void Tick(const float &InDeltaTime) override;

	virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) override;
	
protected:

  UPROPERTY(EditAnywhere)
	FMIRWheelSetting WheelSetting;

	virtual void MoveWheelTick(const float &InDeltaTime);

};