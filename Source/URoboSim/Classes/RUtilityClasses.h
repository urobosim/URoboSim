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
  FJointState() {}

  FJointState(const FString &InJointName, const float &InJointPosition, const float &InJointVelocity)
      : JointName(InJointName), JointPosition(InJointPosition), JointVelocity(InJointVelocity)
  {
  }

public:
  FString JointName;
  float JointPosition;
  float JointVelocity;
};
