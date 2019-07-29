#pragma once

#include "Controller/RController.h"
#include "Controller/RController.h"
#include "RJointController.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URJointController : public URController
{
  GENERATED_BODY()
    public:
    URJointController();

  virtual void Init(ARModel* InModel) override;
  virtual void Tick(float InDeltaTime);

  UPROPERTY(EditAnywhere)
    FString BaseLink;

  UPROPERTY()
    TArray<FTrajectoryPoints> Trajectory;

  UPROPERTY()
    TArray<bool> bTrajectoryPointsReached;

  UPROPERTY()
    bool bFollowTrajectory = false;

  UPROPERTY(EditAnywhere)
    TArray<FString> IgnoreList;


  UPROPERTY()
    TMap<FString, FJointInfo> JointStateList;

  UPROPERTY()
    FTrajectoryStatus TrajectoryStatus;

  virtual void FollowTrajectory();
  virtual void SetJointNames(TArray<FString> InNames);

 protected:

  UPROPERTY()
    ARModel* Model;

  UPROPERTY()
    uint32 TrajectoryPointIndex;

  void UpdateDesiredJointAngle(float InDeltaTime);
  bool CheckTrajectoryStatus();
  void CallculateJointVelocities(float InDeltaTime);
};
