#pragma once

#include "CoreMinimal.h"
// clang-format off
#include "RActionServerParameter.generated.h"
// clang-format on

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URActionServerParameter : public UObject
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  FString ActionName;

  UPROPERTY(EditAnywhere)
  FString ControllerName;
};