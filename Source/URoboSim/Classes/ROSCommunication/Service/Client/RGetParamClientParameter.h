#pragma once

#include "RServiceClientParameter.h"
// clang-format off
#include "RGetParamClientParameter.generated.h"
// clang-format on

USTRUCT()
struct FGetParamArgument
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  FString Name;

  UPROPERTY(EditAnywhere)
  FString Default;
};

UCLASS()
class UROBOSIM_API URGetParamClientParameter : public URServiceClientParameter
{
  GENERATED_BODY()

public:
  URGetParamClientParameter()
  {
    ServiceName = TEXT("rosapi/get_param");
    ServiceType = TEXT("rosapi/GetParam");
  }

public:
  UPROPERTY(EditAnywhere)
  FGetParamArgument GetParamArguments;

  UPROPERTY(EditAnywhere)
  FString ControllerName;
};