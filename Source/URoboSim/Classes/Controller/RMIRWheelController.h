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
	float WheelRadius = 10.f;

	UPROPERTY(EditAnywhere)
	float WheelToCenterSum = 70.65f;

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

  UPROPERTY(VisibleAnywhere)
  TArray<double> WheelVelocities;
};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URMIRWheelController : public URBaseController
{
  GENERATED_BODY()

public:
  URMIRWheelController();

  virtual void Tick(float InDeltaTime) override;

protected:

  UPROPERTY(EditAnywhere)
	FMIRWheelSetting WheelSetting;

	virtual void MoveWheelTick(const float &InDeltaTime);

};