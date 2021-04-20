// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Physics/RModel.h"
#include "ROSUtilities.h"
// clang-format off
#include "RController.generated.h"
// clang-format on

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URControllerParameter : public UObject
{
  GENERATED_BODY()

};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URController : public UObject
{
  GENERATED_BODY()

public:
  // Called every frame
  virtual void Tick(const float &InDeltaTime){};

  virtual void Init();

public:
  ARModel *GetOwner();

  virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) {}

  void AddGoalStatus(const FGoalStatusInfo &GoalStatus) { GoalStatusList.Add(GoalStatus); }

  const FGoalStatusInfo GetGoalStatus() { return GoalStatusList.Last(); }

  const TArray<FGoalStatusInfo> GetGoalStatusList() { return GoalStatusList; }

public:
  bool bCancel = false;

  bool bPublishResult = false;

protected:
  TArray<FGoalStatusInfo> GoalStatusList;

  virtual void CancelAction();

  bool bActive;

private:
  ARModel *Owner;
};