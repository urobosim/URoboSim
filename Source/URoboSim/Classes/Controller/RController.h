// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Physics/RModel.h"
// clang-format off
#include "RController.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URController : public UObject
{
  GENERATED_BODY()

public:
  // Called every frame
  virtual void Tick(float DeltaTime){};

  virtual void Init();

public:
  virtual void SetOwner(ARModel *InOwner) { Owner = InOwner; }

  ARModel *GetOwner();

  // virtual void CancelAction();

public:
  // UPROPERTY()
  // bool bActive;

  // UPROPERTY()
  // bool bCancel = false;

  UPROPERTY()
  bool bPublishResult = false;

  // UPROPERTY()
  // // TArray<FGoalStatusInfo> GoalStatusList;

  // // float CheckAngleRange(float InAngle);

  // UPROPERTY()
  // float CummulatedError = 0;

  // UPROPERTY(EditAnywhere)
  // float HackOffset = 0;

  // UPROPERTY()
  // double ActionDuration;

private:
  UPROPERTY()
  ARModel *Owner;
};