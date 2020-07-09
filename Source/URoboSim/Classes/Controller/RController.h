// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Physics/RModel.h"
#include "RGraspComponent.h"
#include "ConstructorHelpers.h"
#include "Physics/RJoint.h"
/* #include "ROSCommunication/RRosComunication.h" */
#include "RGBDCamera.h"
#include "ROSUtilities.h"
#include "Containers/Queue.h"
#include "RController.generated.h"




UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UPerceivedObject : public UObject
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite,  EditAnywhere, export, noclear)
    FString Name;

    UPROPERTY(BlueprintReadWrite,  EditAnywhere, export, noclear)
    FString Type;

    UPROPERTY()
    FTransform Pose;

    UPROPERTY(BlueprintReadWrite,  EditAnywhere, export, noclear)
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
	double NSecs;
	double Secs;

	void SetTimeStep(double InSecs, double InNSecs)
	{
		Secs = InSecs;
		NSecs = InNSecs;
	};

	double GetTimeAsDouble()
	{
		return Secs + NSecs/100000000;
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
	virtual void Init(ARModel* Model){};
	virtual void CancelAction();


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
};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URBaseController : public URController
{
    GENERATED_BODY()
public:
	URBaseController();

	virtual void Init(ARModel* InModel) override;
	virtual void MoveLinear(FVector InVelocity, float InDeltaTime);
	virtual void MoveLinear(FVector InVelocity);
	virtual void MoveLinearToWorld(FVector InVelocity, float InDeltaTime);
	virtual void Turn(float InVelocity, float InDeltaTime);
	virtual void Turn(float InVelocity);

        virtual void SetLocation(FVector InPosition);
        virtual void SetRotation(FRotator InRotator);
        virtual void SetTransform(FTransform InTransform);
        virtual void AddRelativeLocation(URLink* InLink, FVector InPosition);
        virtual void AddRelativeRotation(URLink* InLink, FRotator InRotation);
        virtual void SetLocationAndRotation(FVector InPosition, FRotator InRotation);

	virtual void Tick(float InDeltaTime) override;

	UPROPERTY(EditAnywhere)
	FString BaseName;
protected:

	virtual void TurnTick(float InDeltaTime);
	virtual void MoveLinearTick(float InDeltaTime);

	UPROPERTY()
	ARModel* Model;


	UPROPERTY(EditAnywhere)
	bool bIsKinematic;

	UPROPERTY()
	float AngularVelocity;

	UPROPERTY()
	FVector LinearVelocity;
};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URCameraController : public URController
{
    GENERATED_BODY()
public:
	URCameraController(){};

	virtual void Init(ARModel* InModel) override;

    virtual void PerceiveObject();

	virtual void Tick(float InDeltaTime) override;

        UPROPERTY(BlueprintReadWrite, Instanced  ,EditAnywhere, export, noclear)
          TArray<UPerceivedObject*> PerceivedObjects;

    UPROPERTY()
    FString TypeToPerceive;

	UPROPERTY()
	ARGBDCamera* Camera;

	UPROPERTY()
        UPerceivedObject* PerceivedObject;
protected:

	UPROPERTY()
	ARModel* Model;

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
	virtual void Init(ARModel* InModel) override;
        virtual void AddTF(FString InFrameName, FTFInfo InTFInfo);
        virtual TMap<FString, FTFInfo> GetTFList();

        virtual bool UpdateFramePoses();
        virtual void SetLinkPose(URLink* InChildLink, URLink* InParentLink, FTransform InPose);

	virtual void Tick(float InDeltaTime) override;

protected:

	UPROPERTY()
	ARModel* Model;

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


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API URControllerComponent : public UActorComponent
{
    GENERATED_BODY()
public:
	URControllerComponent();
	~URControllerComponent();

	virtual URController* ControllerList(FString ControllerName);

	virtual void SetJointVelocities(TArray<FString> InJointNames, TArray<float> InJointVelocities);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	FRControllerContainer Controller;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	ARModel* Model;

};
