#pragma once

#include "ROSBridgeSrvClient.h"
#include "RServiceClient.h"
#include "RGetParamClientParameter.h"
#include "srv/ConfigureJointStatePublisher.h"
#include "Controller/RController.h"
// clang-format off
#include "RGetParamClient.generated.h"
// clang-format on

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