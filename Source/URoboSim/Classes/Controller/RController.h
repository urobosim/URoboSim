// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Physics/RModel.h"
#include "ROSUtilities.h"
#include "RobotInterface.h"
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
class UROBOSIM_API UBasicController : public UObject
{
  GENERATED_BODY()

public:

  UBasicController(){ControllerName = GetName();};

  virtual void Init(){};

  virtual void Tick(const float &InDeltaTime) {}


  virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) {}

  const TArray<FGoalStatusInfo> GetGoalStatusList() const { return GoalStatusList; }

  void AddGoalStatus(const FGoalStatusInfo &GoalStatus) { GoalStatusList.Add(GoalStatus); }

  bool bActive;

  bool bCancel = false;

  bool bPublishResult = false;

  FActionFinishedDelegate ActionFinished;

  UPROPERTY(VisibleAnywhere)
    FString ControllerName;

protected:
  virtual void CancelAction();

  TArray<FGoalStatusInfo> GoalStatusList;

  double ActionDuration;



};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URController : public UBasicController, public IRobotInterface
{
  GENERATED_BODY()

public:

  virtual void Init() override;

  void SetOwner();

};
