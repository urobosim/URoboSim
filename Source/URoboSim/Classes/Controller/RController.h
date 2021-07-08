// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Physics/RModel.h"
#include "ROSUtilities.h"
// clang-format off
#include "RController.generated.h"
// clang-format on

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionFinishedDelegate, FGoalStatusInfo, StatusInfo);

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
  virtual void Tick(const float &InDeltaTime) {}

  virtual void Init();

  URController(){ControllerName = GetName();};

public:
  ARModel *GetOwner() const { return Owner; }

  void SetOwner(UObject *InOwner) { Owner = Cast<ARModel>(InOwner); }

  void SetOwner();

  virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) {}

  const TArray<FGoalStatusInfo> GetGoalStatusList() const { return GoalStatusList; }

  void AddGoalStatus(const FGoalStatusInfo &GoalStatus) { GoalStatusList.Add(GoalStatus); }

public:
  bool bActive;

  bool bCancel = false;

  bool bPublishResult = false;

  FActionFinishedDelegate ActionFinished;

  UPROPERTY(VisibleAnywhere)
    FString ControllerName;

protected:
  virtual void CancelAction();

protected:
  TArray<FGoalStatusInfo> GoalStatusList;

  double ActionDuration;

private:
  ARModel *Owner;
};
