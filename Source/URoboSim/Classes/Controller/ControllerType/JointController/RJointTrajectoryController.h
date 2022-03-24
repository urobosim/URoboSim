#pragma once

#include "RJointController.h"
// clang-format off
#include "RJointTrajectoryController.generated.h"
// clang-format on

UENUM()
enum class UJointControllerState : uint8
{
  Normal,
  FollowJointTrajectory
};

USTRUCT()
struct FTrajectoryPoints
{
  GENERATED_BODY()
public:
  FTrajectoryPoints() {}

  FTrajectoryPoints(const float &InSecs, const float &InNSecs, const TArray<FString> &InJointNames, const TArray<double> &InPositions, const TArray<double> &InVelocities)
  {
    SetTimeStep(InSecs, InNSecs);
    for (int32 i = 0; i < InJointNames.Num(); i++)
    {
      JointStates.Add(InJointNames[i], FJointState(InPositions[i], InVelocities[i]));
    }
  }

public:
  TMap<FString, FJointState> JointStates;
  double StartTime;
  double NSecs;
  double Secs;

  void SetTimeStep(double InSecs, double InNSecs)
  {
    Secs = InSecs;
    NSecs = InNSecs;
  };

  double GetTimeAsDouble()
  {
    return Secs + NSecs / 1000000000;
  };

  void Reset()
  {
    StartTime = 0.0;
    JointStates.Empty();
    NSecs = 0;
    Secs = 0;
  };
};

UCLASS()
class UROBOSIM_API URJointTrajectoryControllerParameter : public URJointControllerParameter
{
  GENERATED_BODY()

};

UCLASS()
class UROBOSIM_API URJointTrajectoryController : public URJointController
{
  GENERATED_BODY()

public:
  URJointTrajectoryController();

public:
  virtual void Tick(const float &InDeltaTime);

public:
  TArray<FTrajectoryPoints> Trajectory;


  UFUNCTION()
  void FollowJointTrajectory(double InActionStartTime, FGoalStatusInfo InGoalInfo, TArray<FTrajectoryPoints> InTrajectory);

  UJointControllerState GetState() const { return State; }

  UPROPERTY(EditAnywhere)
    float SpeedFactor = 1.0;

protected:
  void Reset();

  void UpdateDesiredJointAngle(float InDeltaTime);

  virtual bool CheckTrajectoryGoalReached();

  virtual bool CheckTrajectoryPoint();

  float CalculateJointVelocity(float InDeltaTime, FString InJointName);

  void SetJointVelocities(float InDeltaTime);

  virtual bool SwitchToNormal();

  virtual bool SwitchToFollowJointTrajectory();

  void SetDesiredJointState(FString JointName, float InJointState);

protected:
  UPROPERTY(VisibleAnywhere)
  UJointControllerState State;

  FTrajectoryPoints OldTrajectoryPoints;

  float ActionStartTime;

  int32 TrajectoryPointIndex;
};
