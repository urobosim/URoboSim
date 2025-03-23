#pragma once

#include "Physics/RModel.h"
#include "Controller/ControllerType/JointController/RJointController.h"
// clang-format off
#include "RGripperControllerBase.generated.h"
// clang-format on

USTRUCT()
struct FGripperParts
{
  GENERATED_BODY()
public:

  UPROPERTY(EditAnywhere)
  TArray<FString> GripperPartNames;
};

USTRUCT()
struct FGraspObjects
{
  GENERATED_BODY()
public:

  //Components of the gripper that collided with the grasp object
  UPROPERTY(VisibleAnywhere)
  TArray<UPrimitiveComponent*> HitComps;
};

USTRUCT()
struct FGraspingParts
{
  GENERATED_BODY()
public:
  FGraspingParts(){};

  FGraspingParts(UPrimitiveComponent* InPart1, UPrimitiveComponent* InPart2) :
  Part1(InPart1), Part2(InPart2)
  {
    if(!InPart2 && !InPart1)
      {
        UE_LOG(LogTemp, Error, TEXT("Part1 or Part2 nullptr"));
        return;
      }
  };

  float GetDistance()
  {
    if(!Part2 && !Part1)
      {
        UE_LOG(LogTemp, Error, TEXT("GraspingParts: Part1 or Part2 nullptr"));
        return -1;
      }
    FVector DistV = Part1->GetComponentLocation() - Part2->GetComponentLocation();
    return FMath::Abs(DistV.Size());
  };

  UPROPERTY(VisibleAnywhere)
  UPrimitiveComponent* Part1 = nullptr;

  UPROPERTY(VisibleAnywhere)
  UPrimitiveComponent* Part2 = nullptr;
};

USTRUCT()
struct FGripperJointAssociation
{
  GENERATED_BODY()
public:
  UPROPERTY(VisibleAnywhere)
  TArray<URLink*> ReferenceFrames;

  UPROPERTY(EditAnywhere)
  TArray<FString> ReferenceFrameNames;

  bool InitRefLinks(ARModel* Model);

};

USTRUCT()
struct FGraspComponentSetting
{
  GENERATED_BODY()
public:
  UPROPERTY(EditAnywhere)
  FString GripperName;

  UPROPERTY(EditAnywhere)
  FVector ToolCenterPoint = FVector(15.0f, 0.0f, 0.0f);

  UPROPERTY(EditAnywhere)
  float Radius = 3.0;
};

UCLASS()
class UROBOSIM_API URGripperControllerBaseParameter : public URControllerParameter
{
  GENERATED_BODY()
public:
    //tracebot_right_gripper_joint_intermediate_1
    //tracebot_left_gripper_distal_phalanx_4
    //tracebot_left_gripper_intermediate_phalanx_4
    //tracebot_left_gripper_proximal_phalanx_4
  UPROPERTY(EditAnywhere)
  TArray<FString> GripperJointNames;

  UPROPERTY(EditAnywhere)
    TMap<FString,FGripperJointAssociation> GripperJointReferenceFrames;

  UPROPERTY(EditAnywhere)
  FString GraspComponentName;

  UPROPERTY(EditAnywhere)
    TMap<FString,FGripperParts> GraspPartNames;

  UPROPERTY(EditAnywhere)
  FGraspComponentSetting GraspCompSetting;

  UPROPERTY(EditAnywhere)
  UJointControllerMode Mode;

  UPROPERTY(EditAnywhere)
  FEnableDrive EnableDrive;

  UPROPERTY(EditAnywhere)
    bool bOverwriteConfig = true;

  UPROPERTY(EditAnywhere)
    bool bInvertGraspCondition = false;
};

UCLASS()
class UROBOSIM_API URGripperControllerBase : public URController
{
  GENERATED_BODY()

public:
  URGripperControllerBase();

public:
  virtual void Init() override;

  virtual void SetControllerParameters(URControllerParameter *&ControllerParameters) override;

  virtual bool Grasp();

  virtual void Release();

  virtual void Tick(const float &InDeltaTime);

public:
  URGraspComponent *GraspComponent;

  UPROPERTY(EditAnywhere)
  FGraspComponentSetting GraspCompSetting;

  UPROPERTY(EditAnywhere)
  URLink* ReferenceLink;

  float OldPosition;

  UPROPERTY(EditAnywhere)
    TArray<FString> GripperJointNames;

  UPROPERTY(EditAnywhere)
    TMap<FString,FGripperParts> GraspPartNames;

  UPROPERTY(VisibleAnywhere)
    TMap<UPrimitiveComponent*, FString> GraspParts;

  UPROPERTY(VisibleAnywhere)
    FGraspingParts GraspingParts;

  //Objects that were hit by the gripper
  UPROPERTY(VisibleAnywhere)
    TMap<UPrimitiveComponent*, FGraspObjects> HitObjects;

  UPROPERTY(EditAnywhere)
    TMap<FString,FGripperJointAssociation> GripperJointReferenceFrames;

  UPROPERTY()
  TArray<URJoint*> GripperJoints;

  UPROPERTY(EditAnywhere)
    bool bInvertGraspCondition = false;

  UPROPERTY(EditAnywhere)
  UJointControllerMode Mode;

  UPROPERTY(EditAnywhere)
    bool bOverwriteConfig = true;

  UPROPERTY(EditAnywhere)
  FEnableDrive EnableDrive;

protected:
  URJointController *JointController;

  UPROPERTY(EditAnywhere)
  FString GraspComponentName;

  UPROPERTY()
    bool bDebugMode = false;

  int GraspInversion = 1;

  UFUNCTION()
    void GraspHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
                            UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
