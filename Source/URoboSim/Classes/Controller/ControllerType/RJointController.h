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
  double StartTime;
  TArray<double> Points;
  TArray<double> Velocities;
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
    Points.Empty();
    Velocities.Empty();
    NSecs = 0;
    Secs = 0;
  };
};

USTRUCT()
struct FTrajectoryStatus
{
  GENERATED_BODY()

public:
  TArray<FString> JointNames;
  TArray<double> Position;
  TArray<double> Desired;
  TArray<double> Error;
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
  UPROPERTY(VisibleAnywhere)
  UJointControllerState State;

  UPROPERTY(EditAnywhere)
  FEnableDrive EnableDrive;

  //   UPROPERTY(EditAnywhere)
  //   float RevolutAccuracy;

  //   UPROPERTY(EditAnywhere)
  //   float PrismaticAccuracy;

  UPROPERTY(EditAnywhere)
  bool bDisableCollision;

  //   UPROPERTY()
  //   TArray<FTrajectoryPoints> Trajectory;

  UPROPERTY(EditAnywhere)
  TMap<FString, FJointState> DesiredJointStates;

  //   UPROPERTY()
  //   TArray<bool> bTrajectoryPointsReached;

  //   UPROPERTY(EditAnywhere)
  //   float MaxJointAngularVel;

  //   UPROPERTY()
  //   FTrajectoryStatus TrajectoryStatus;

  //   virtual void FollowTrajectory();
  //   virtual void SetJointNames(TArray<FString> InNames);
  virtual void SetMode();

  //   UJointControllerState GetState();

  //   virtual void SetDesiredJointState(FString JointName, float InJointState);

protected:
  UPROPERTY(EditAnywhere)
  UJointControllerMode Mode;

  void SetJointState(float DeltaTime);

  //   UPROPERTY(EditAnywhere)
  //   float SpeedFactorHack = 1;
  //   virtual void MoveJoints(float InDeltaTime);
  //   virtual void MoveJointsDynamic(float InDeltaTime);
  //   virtual void MoveJointsKinematic();

  //   UPROPERTY()
  //   FTrajectoryPoints OldTrajectoryPoints;

  //   UPROPERTY()
  //   uint32 TrajectoryPointIndex;

  //   void UpdateDesiredJointAngle(float InDeltaTime);
  //   virtual bool CheckTrajectoryGoalReached();
  //   virtual bool CheckTrajectoryPoint();

  //   void SetJointVelocities(float InDeltaTime);
};