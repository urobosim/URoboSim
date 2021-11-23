#pragma once

#include "Controller/RController.h"
// clang-format off
#include "RTFController.generated.h"
// clang-format on

USTRUCT()
struct FTFInfo
{
  GENERATED_BODY()
public:

  UPROPERTY(VisibleAnywhere)
  FString ParentFrame;

  UPROPERTY(VisibleAnywhere)
  FTransform Pose;
  // FString ChildFrame;
  // FORCEINLINE FTransform() : ParentFrame(""), Pose(FTransform()){};
  // FORCEINLINE explicit FTFInfo() : ParentFrame(InParentFrame), Pose(InPose){};
};

USTRUCT()
struct FTFActors
{
  GENERATED_BODY()
public:
  UPROPERTY(VisibleAnywhere)
  USceneComponent* Parent;
  UPROPERTY(VisibleAnywhere)
  USceneComponent* Child;
  UPROPERTY(VisibleAnywhere)
  FTransform ParentTransform;
  UPROPERTY(VisibleAnywhere)
  bool bParentFound;
  // FString ChildFrame;
  // FORCEINLINE FTransform() : ParentFrame(""), Pose(FTransform()){};
  // FORCEINLINE explicit FTFInfo() : ParentFrame(InParentFrame), Pose(InPose){};
};

UCLASS()
class UROBOSIM_API URTFControllerParameter : public URControllerParameter
{
  GENERATED_BODY()

};

UCLASS(Blueprintable, DefaultToInstanced, hidecategories = Object, editinlinenew)
class UROBOSIM_API UTFHander : public UObject
{
  GENERATED_BODY()

public:

  UPROPERTY(VisibleAnywhere)
  TMap<FString, FTFActors> TFActors;

  void UpdateTF(const FString& TFFrame, const FTFInfo& InTFInfo);

  void SetWorld(UWorld* InWorld){World = InWorld;};

  UPROPERTY()
    FString FixedFrame;

  UPROPERTY()
    TMap<FString, FTransform> Frames;

 protected:

  virtual void SetPose(const FTransform& InPose){};
  virtual void SearchFrames(const FTransform& InPose){};

  UPROPERTY()
  FString ChildFrame;

  UPROPERTY()
  FString ParentFrame;

  UPROPERTY()
  UWorld *World;

};

UCLASS(Blueprintable, DefaultToInstanced, hidecategories = Object, editinlinenew)
class UROBOSIM_API UTFAActorHander : public UTFHander
{
  GENERATED_BODY()

public:

  virtual void SetPose(const FTransform& InPose) override;
  virtual void SearchFrames(const FTransform& InPose) override;
};

UCLASS()
class UROBOSIM_API UTFRobotHander : public UTFHander
{
  GENERATED_BODY()

public:

  virtual void SetPose(const FTransform& InPose) override;
  virtual void SearchFrames(const FTransform& InPose) override;

 protected:

  UPROPERTY()
    TMap<FString, FTransform> OffsetList;

  UPROPERTY()
    TMap<FString, FTFInfo> VirtualFrames;
};

UCLASS()
class UROBOSIM_API UTFController : public UBasicController
{
  GENERATED_BODY()

public:

  virtual void Init() override;

  virtual TMap<FString, FTFInfo> GetTFList();

  virtual void AddTF(FString InFrameName, FTFInfo InTFInfo);

  virtual void Tick(const float &InDeltaTime) override;

  virtual bool UpdateFramePoses();

  void SetWorld(UWorld* InWorld);

protected:
  UPROPERTY(EditAnywhere)
    FString FixedFrame;

  UPROPERTY(EditAnywhere)
  float UpdateRate;

  UPROPERTY()
  float Time;

  UPROPERTY(VisibleAnywhere)
  TMap<FString, FTFInfo> TFList;

  // UPROPERTY()
  // TMap<FString, FTFActors> TFActors;

  UPROPERTY(EditAnywhere)
    TMap<FString, FTransform> Frames;

  UPROPERTY()
  UWorld *World;

  UPROPERTY(EditAnywhere, Instanced)
    TArray<UTFHander*> TFHanderList;
};

// UCLASS()
// class UROBOSIM_API URTFController : public URController
// {
//   GENERATED_BODY()

// public:
//   virtual void Init() override;

//   virtual bool UpdateFramePoses();

//   virtual void SetLinkPose(UStaticMeshComponent *InChildLink, UStaticMeshComponent *InParentLink, FTransform InPose);

// protected:
//   UPROPERTY(EditAnywhere)
//     FString FixedFrame;

//   UPROPERTY(EditAnywhere)
//   float UpdateRate;

//   float Time;

//   TMap<FString, FTFInfo> TFList;
// };
