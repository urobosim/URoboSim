#pragma once

#include "CoreMinimal.h"
#include "RUtilityClasses.generated.h"

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

struct FJointState
{
public:
	TArray<FString> JointNames;
	TArray<float> JointPositions;
	TArray<float> JointVelocities;
};
