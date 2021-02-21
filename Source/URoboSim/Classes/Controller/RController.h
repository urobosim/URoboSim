// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "ConstructorHelpers.h"
#include "Physics/RJoint.h"
#include "Physics/RModel.h"
#include "RGraspComponent.h"
//#include "ROSCommunication/RRosComunication.h"
#include "Containers/Queue.h"
#include "Conversions.h"
#include "RGBDCamera.h"
#include "ROSUtilities.h"
// clang-format off
#include "RController.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UPerceivedObject : public UObject
{
  GENERATED_BODY()
public:
  UPROPERTY(BlueprintReadWrite, EditAnywhere, export, noclear)
  FString Name;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, export, noclear)
  FString Type;

  UPROPERTY()
  FTransform Pose;

  UPROPERTY(BlueprintReadWrite, EditAnywhere, export, noclear)
  FTransform PoseWorld;
};

USTRUCT()
struct FTFInfo
{
  GENERATED_BODY()
public:
  FString ParentFrame;
  FTransform Pose;
  // FString ChildFrame;
  // FORCEINLINE FTransform() : ParentFrame(""), Pose(FTransform()){};
  // FORCEINLINE explicit FTFInfo() : ParentFrame(InParentFrame), Pose(InPose){};
};

USTRUCT()
struct FJointInfo
{
  GENERATED_BODY()
public:
  float JointPosition;
  float JointVelocity;
  float JointEffort;
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
class UROBOSIM_API URController : public UObject
{
  GENERATED_BODY()
public:
  virtual void Tick(float InDeltaTime){};
  virtual void Init(){};
  virtual void SetOwner(ARModel* Model);
  virtual void CancelAction();
  const ARModel* GetOwner();

  UPROPERTY()
  bool bActive;

  UPROPERTY()
  bool bCancel = false;

  UPROPERTY()
  bool bPublishResult = false;

  UPROPERTY()
  TArray<FGoalStatusInfo> GoalStatusList;

  // float CheckAngleRange(float InAngle);

  UPROPERTY()
  float CummulatedError = 0;

  UPROPERTY(EditAnywhere)
  float HackOffset = 0;

  UPROPERTY()
  double ActionDuration;

private:
  UPROPERTY()
  ARModel* Owner;
};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URCameraController : public URController
{
  GENERATED_BODY()
public:
  URCameraController(){};

  virtual void Init() override;

  virtual void PerceiveObject();

  virtual void Tick(float InDeltaTime) override;

  UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
  TArray<UPerceivedObject*> PerceivedObjects;

  UPROPERTY()
  FString TypeToPerceive;

  UPROPERTY()
  ARGBDCamera* Camera;

  UPROPERTY()
  UPerceivedObject* PerceivedObject;

protected:
  UPROPERTY(EditAnywhere)
  FString CameraRef;

  UPROPERTY(EditAnywhere)
  FString CameraName;

  UPROPERTY(EditAnywhere)
  FTransform PoseOffset;
};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URTFController : public URController
{
  GENERATED_BODY()
public:
  virtual void Init() override;
  virtual void AddTF(FString InFrameName, FTFInfo InTFInfo);
  virtual TMap<FString, FTFInfo> GetTFList();

  virtual bool UpdateFramePoses();
  virtual void SetLinkPose(URStaticMeshComponent* InChildLink, URStaticMeshComponent* InParentLink, FTransform InPose);

  virtual void Tick(float InDeltaTime) override;

protected:
  UPROPERTY(EditAnywhere)
  float UpdateRate;

  UPROPERTY()
  float Time;

  UPROPERTY()
  TMap<FString, FTFInfo> TFList;
};

USTRUCT(Blueprintable)
struct FRControllerContainer
{
  GENERATED_BODY()

public:
  UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
  TMap<FString, URController*> ControllerList;
};
