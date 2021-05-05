#pragma once

#include "CoreMinimal.h"
// clang-format off
#include "RUtilityClasses.generated.h"
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
struct FJointState
{
  GENERATED_BODY()

public:
  FJointState() : FJointState(0.f, 0.f) {}

  FJointState(const float &InJointPosition, const float &InJointVelocity)
      : JointPosition(InJointPosition), JointVelocity(InJointVelocity)
  {
  }

public:
  UPROPERTY(EditAnywhere)
  float JointPosition;

  UPROPERTY(EditAnywhere)
  float JointVelocity;
};
