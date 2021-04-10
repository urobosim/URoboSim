// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Physics/RModel.h"
#include "RControllerParameter.h"
#include "ROSUtilities.h"
// clang-format off
#include "RController.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URController : public UObject
{
  GENERATED_BODY()

public:
  // Called every frame
  virtual void Tick(const float &InDeltaTime){};

  virtual void Init();

public:
  void SetOwner(ARModel *InOwner) { Owner = InOwner; }

  ARModel *GetOwner();

  virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) {}

  void AddGoalStatus(const FGoalStatusInfo &GoalStatus) { GoalStatusList.Add(GoalStatus); }

  const FGoalStatusInfo GetGoalStatus() { return GoalStatusList.Last(); }

  const TArray<FGoalStatusInfo> GetGoalStatusList() { return GoalStatusList; }

public:
  UPROPERTY()
  bool bCancel = false;

  UPROPERTY()
  bool bPublishResult = false;

protected:
  UPROPERTY()
  TArray<FGoalStatusInfo> GoalStatusList;

  virtual void CancelAction();

  UPROPERTY()
  bool bActive;

private:
  UPROPERTY()
  ARModel *Owner;
};