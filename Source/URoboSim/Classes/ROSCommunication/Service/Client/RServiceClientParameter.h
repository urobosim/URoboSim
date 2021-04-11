#pragma once

#include "CoreMinimal.h"
// clang-format off
#include "RServiceClientParameter.generated.h"
// clang-format on

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URServiceClientParameter : public UObject
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  FString ServiceName;

  UPROPERTY(EditAnywhere)
  FString ServiceType;
};