#pragma once

#include "ROSBridgeSrvClient.h"
#include "RServiceClient.h"
#include "srv/ConfigureJointStatePublisher.h"
#include "Controller/RController.h"
// clang-format off
#include "RGetParamClient.generated.h"
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

UCLASS()
class UROBOSIM_API URGetParamClient : public URServiceClient
{
  GENERATED_BODY()

public:
  URGetParamClient();

public:
  void CallService() override;

protected:
  virtual void Init() override;

public:
  virtual void SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters) override;

public:
  UPROPERTY(EditAnywhere)
  FGetParamArgument GetParamArguments;

  UPROPERTY(EditAnywhere)
  FString ControllerName;

  TSharedPtr<class FRGetParamClientCallback> GetParamClient;
  TSharedPtr<rosapi::GetParam::Request> Request;
  TSharedPtr<rosapi::GetParam::Response> Response;
};

class FRGetParamClientCallback : public FROSBridgeSrvClient
{
public:
  FRGetParamClientCallback(const FString &InServiceName, const FString &InServiceType, URController *InController);

  void Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;

protected:
  virtual void Callback(){}

protected:
  URController *Controller;

  FString ParamString;
};