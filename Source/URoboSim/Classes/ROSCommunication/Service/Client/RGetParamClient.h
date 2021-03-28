#pragma once

#include "ROSBridgeSrvClient.h"
#include "ROSCommunication/Service/Client/RServiceClient.h"
#include "srv/ConfigureJointStatePublisher.h"
#include "Controller/ControllerType/RJointController.h"
#include "Controller/RControllerComponent.h"
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
class UROBOSIM_API URGetParamClient : public URServiceClient
{
  GENERATED_BODY()

public:
  URGetParamClient();

public:
  virtual void CallService() override;

protected:
  virtual void Init() override;

public:
  UPROPERTY(EditAnywhere)
  FGetParamArgument GetParamArgument;

protected:
  UPROPERTY()
  URControllerComponent *ControllerComponent;

  TSharedPtr<class FRGetParamClientCallback> GetParamClient;
  TSharedPtr<rosapi::GetParam::Request> Request;
  TSharedPtr<rosapi::GetParam::Response> Response;
};

class FRGetParamClientCallback : public FROSBridgeSrvClient
{
public:
  FRGetParamClientCallback(const FString &InServiceName, const FString &InServiceType, UObject *InController);

  virtual void Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;

protected:
  virtual void Callback(){}

protected:
  URJointController *JointController;

  FString ParamString;
};