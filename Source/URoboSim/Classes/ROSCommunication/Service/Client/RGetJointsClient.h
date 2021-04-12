#pragma once

#include "ROSCommunication/Service/Client/RGetParamClient.h"
// clang-format off
#include "RGetJointsClient.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URGetJointsClientParameter : public URGetParamClientParameter
{
  GENERATED_BODY()

public:
  URGetJointsClientParameter()
  {
    GetParamArguments.Name = TEXT("/hardware_interface/joints");
    ControllerName = TEXT("JointController");
  }
};

UCLASS()
class UROBOSIM_API URGetJointsClient final : public URGetParamClient
{
  GENERATED_BODY()

public:
  URGetJointsClient();

protected:
  void Init() override;
};

class FRGetJointsClientCallback final : public FRGetParamClientCallback
{
public:
  FRGetJointsClientCallback(const FString &InServiceName, const FString &InServiceType, URController *InController);

protected:
  void Callback() override;
};