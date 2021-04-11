#pragma once

#include "RGetParamClientParameter.h"
// clang-format off
#include "RGetJointsClientParameter.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URGetJointsClientParameter : public URGetParamClientParameter
{
  GENERATED_BODY()

public:
  URGetJointsClientParameter()
  {
    GetParamArguments.Name = TEXT("hardware_interface/joints");
    ControllerName = TEXT("JointController");
  }
};