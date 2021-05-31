// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "RBaseController.h"
// clang-format off
#include "ROmniwheelController.generated.h"
// clang-format on

USTRUCT()
struct FWheelSetting
{
  GENERATED_BODY()

public:
	FWheelSetting()
	{
		WheelVelocities.Init(0.0, 4);
	}
	
public:
  UPROPERTY(EditAnywhere)
	float WheelRadius = 10.f;

	UPROPERTY(EditAnywhere)
	float WheelToCenterSum = 70.65f;

	UPROPERTY(EditAnywhere)
	FString WheelFrontLeft = FString(TEXT("wheel_front_left"));

	UPROPERTY(EditAnywhere)
	FString WheelFrontRight = FString(TEXT("wheel_front_right"));

	UPROPERTY(EditAnywhere)
	FString WheelBackLeft = FString(TEXT("wheel_back_left"));

	UPROPERTY(EditAnywhere)
	FString WheelBackRight = FString(TEXT("wheel_back_right"));

  UPROPERTY(VisibleAnywhere)
  TArray<double> WheelVelocities;
};

UCLASS()
class UROBOSIM_API UROmniwheelControllerParameter : public URBaseControllerParameter
{
  GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FWheelSetting WheelSetting;
};

UCLASS()
class UROBOSIM_API UROmniwheelController : public URBaseController
{
  GENERATED_BODY()

public:
  UROmniwheelController();

	virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) override;

  virtual void Tick(const float &InDeltaTime) override;

protected:

  UPROPERTY(EditAnywhere)
	FWheelSetting WheelSetting;

	virtual void MoveWheelTick(const float &InDeltaTime);

};