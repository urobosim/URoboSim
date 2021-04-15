#pragma once

#include "CoreMinimal.h"
// clang-format off
#include "RSubscriberParameter.generated.h"
// clang-format on

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URSubscriberParameter : public UObject
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  FString Topic;

  UPROPERTY(VisibleAnywhere)
  FString MessageType;

  UPROPERTY(EditAnywhere)
  FString ControllerName;
};