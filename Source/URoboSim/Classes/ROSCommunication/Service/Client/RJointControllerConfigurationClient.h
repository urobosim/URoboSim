#pragma once

#include "RServiceClient.h"
#include "ROSBridgeSrvClient.h"
#include "srv/ConfigureJointStatePublisher.h"
#include "Controller/ControllerType/JointController/RJointController.h"
// clang-format off
#include "RJointControllerConfigurationClient.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointControllerConfigurationClientParameter final : public URServiceClientParameter
{
  GENERATED_BODY()

public:
  URJointControllerConfigurationClientParameter()
  {
    ServiceName = TEXT("rosapi/get_param");
    ServiceType = TEXT("rosapi/GetParam");
    JointParamTopic = TEXT("/whole_body_controller/joints");
    LimitParamTopic = TEXT("/robot_description");
    JointControllerName = TEXT("JointController");
  }

public:
  UPROPERTY(EditAnywhere)
  FString JointControllerName;

  UPROPERTY(EditAnywhere)
  FString JointParamTopic;

  UPROPERTY(EditAnywhere)
  FString LimitParamTopic;
};

UCLASS()
class UROBOSIM_API URJointControllerConfigurationClient final : public URServiceClient
{
  GENERATED_BODY()

public:
  URJointControllerConfigurationClient();

public:
  void CallService() override;

  void SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters) override;

protected:
  void CreateServiceClient() override;

public:
  UPROPERTY(EditAnywhere)
  FString JointControllerName;

  UPROPERTY(EditAnywhere)
  FString JointParamTopic;

  UPROPERTY(EditAnywhere)
  FString LimitParamTopic;

private:
  TSharedPtr<class FRJointControllerConfigurationClient> JointServiceClient;

  TSharedPtr<class FRJointLimitControllerConfigurationClient> JointLimitServiceClient;

  TSharedPtr<rosapi::GetParam::Request> JointRequest;

  TSharedPtr<rosapi::GetParam::Response> JointResponse;

  TSharedPtr<rosapi::GetParam::Request> LimitRequest;
  
  TSharedPtr<rosapi::GetParam::Response> LimitResponse;
};

class FRJointControllerConfigurationClient final : public FROSBridgeSrvClient
{
public:
  FRJointControllerConfigurationClient(URJointController *InJointController, TMap<FString, FJointState> *OutJointNames, const FString &InName, const FString &InType);

  void Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;

private:
  TMap<FString, FJointState> *JointNames;

  URJointController *JointController;
};

class FRJointLimitControllerConfigurationClient final : public FROSBridgeSrvClient
{
public:
  FRJointLimitControllerConfigurationClient(TMap<FString, FJointState> *OutJointNames, ARModel *InModel, const FString &InName, const FString &InType);

  void Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;

private:
  TMap<FString, FJointState> *JointNames;

  ARModel *Model;
};