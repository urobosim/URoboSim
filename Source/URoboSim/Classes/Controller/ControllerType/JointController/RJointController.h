#pragma once

#include "Controller/RController.h"
#include "RJointController.generated.h"

UENUM()
enum class UJointControllerState : uint8
{
  Normal,
  FollowJointTrajectory,
  Off
};

UENUM()
enum class UJointControllerMode : uint8
{
  Dynamic,
  Kinematic
};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URJointController : public URController
{
  GENERATED_BODY()
    public:
    URJointController();

  virtual void Init() override;
  virtual void Tick(float InDeltaTime);

  UPROPERTY(VisibleAnywhere)
    UJointControllerState State;

  UPROPERTY(EditAnywhere)
    FString BaseLink;

  UPROPERTY(EditAnywhere)
    float RevolutAccuracy;

  UPROPERTY(EditAnywhere)
    float PrismaticAccuracy;


  UPROPERTY(EditAnywhere)
    bool bDisableCollision;

  UPROPERTY()
    TArray<FTrajectoryPoints> Trajectory;

  UPROPERTY(EditAnywhere)
    TMap<FString,float> DesiredJointState;

  UPROPERTY()
    TArray<bool> bTrajectoryPointsReached;


  UPROPERTY(EditAnywhere)
    float MaxJointAngularVel;

  UPROPERTY()
    FTrajectoryStatus TrajectoryStatus;

  virtual void FollowTrajectory(double InActionStart, FGoalStatusInfo InGoalInfo, TArray<FString> InJointNames, TArray<FTrajectoryPoints> InTrajectory);
  virtual void SetJointNames(TArray<FString> InNames);
  virtual void SwitchMode(UJointControllerMode InMode, bool IsInit = false);

  UJointControllerState GetState();

  virtual void SetDesiredJointState(FString JointName, float InJointState);

  virtual void EnableMotor(bool bEnableTrue);

 protected:
  UPROPERTY(EditAnywhere)
    UJointControllerMode Mode;

  UPROPERTY(EditAnywhere)
    float SpeedFactorHack = 1;
  virtual void MoveJoints(float InDeltaTime);
  virtual void MoveJointsDynamic(float InDeltaTime);
  virtual void MoveJointsKinematic();

  UPROPERTY()
    FTrajectoryPoints OldTrajectoryPoints;

  UPROPERTY()
    uint32 TrajectoryPointIndex;

  void UpdateDesiredJointAngle(float InDeltaTime);
  virtual bool CheckTrajectoryGoalReached();
  virtual bool CheckTrajectoryPoint();
  float CallculateJointVelocity(float InDeltaTime, FString InJointName);
  void SetJointVelocities(float InDeltaTime);
  virtual bool SwitchToNormal();
  virtual bool SwitchToFollowJointTrajectory();
};
