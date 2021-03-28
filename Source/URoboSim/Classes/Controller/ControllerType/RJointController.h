#pragma once

#include "Controller/RController.h"
// clang-format off
#include "RJointController.generated.h"
// clang-format on

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

  UPROPERTY(VisibleAnywhere)
  float StartTime;

  UPROPERTY(VisibleAnywhere)
  float Secs;

  UPROPERTY(VisibleAnywhere)
  float NSecs;

  UPROPERTY(VisibleAnywhere)
  TMap<FString, FJointState> JointStates;

public:
  void SetTimeStep(const float &InSecs, const float &InNSecs)
  {
    Secs = InSecs;
    NSecs = InNSecs;
  };

  const float GetTimeAsFloat() const
  {
    return Secs + NSecs / 1000000000;
  };

  void Reset()
  {
    StartTime = 0.f;
    Secs = 0;
    NSecs = 0;
    JointStates.Empty();
  };
};

USTRUCT()
struct FTrajectoryStatus
{
  GENERATED_BODY()

public:
  FTrajectoryStatus() {}

  FTrajectoryStatus(const FString &InJointName) : JointName(InJointName), CurrentPosition(0.f), DesiredPosition(0.f), ErrorPosition(0.f) {}

  FString JointName;
  float CurrentPosition;
  float DesiredPosition;
  float ErrorPosition;
};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URJointController : public URController
{
  GENERATED_BODY()

public:
  URJointController();

public:
  virtual void Tick(float DeltaTime) override;

  virtual void Init() override;

public:
  virtual void SetJointNames(const TArray<FString> &InNames);

  virtual void SetMode();

  virtual const TArray<FTrajectoryStatus> GetTrajectoryStatusArray() const { return TrajectoryStatusArray; }

  virtual void FollowJointTrajectory();

public:
  UPROPERTY(VisibleAnywhere)
  UJointControllerState State;

  UPROPERTY(EditAnywhere)
  FEnableDrive EnableDrive;

  UPROPERTY(EditAnywhere)
  bool bDisableCollision;

  UPROPERTY(EditAnywhere)
  TMap<FString, FJointState> DesiredJointStates;

  UPROPERTY()
  TArray<FTrajectoryPoints> DesiredTrajectory;

protected:
  UPROPERTY(EditAnywhere)
  UJointControllerMode Mode;

  UPROPERTY()
  TArray<FTrajectoryStatus> TrajectoryStatusArray;

  UPROPERTY()
  uint32 TrajectoryPointIndex;

  UPROPERTY()
  float ActionDuration;

protected:
  virtual void SetJointState(float DeltaTime);

  virtual bool CheckTrajectoryPoint();

  virtual bool CheckTrajectoryGoalReached();

  virtual void SetDesiredJointState(float DeltaTime);

private:
  UPROPERTY()
  FTrajectoryPoints LastTrajectoryPoints;
};