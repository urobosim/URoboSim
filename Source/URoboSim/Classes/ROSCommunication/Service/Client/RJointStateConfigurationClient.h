#pragma once

#include "Physics/RModel.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvClient.h"
#include "RServiceClient.h"
#include "srv/ConfigureJointStatePublisher.h"
// clang-format off
#include "RJointStateConfigurationClient.generated.h"
// clang-format on

class FRJointStateConfigurationClient final : public FROSBridgeSrvClient
{
public:
  FRJointStateConfigurationClient(TArray<FString> *OutJointNames, const FString &InName, const FString &InType);

  void Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;

private:
  TArray<FString> *JointNames;
};

UCLASS()
class UROBOSIM_API URJointStateConfigurationClient : public URServiceClient
{
  GENERATED_BODY()

public:
  virtual void Init(UObject *InModel, TArray<FString> *OutArray) override;

public:
  virtual void CallService() override;

protected:
  UPROPERTY(EditAnywhere)
  FString JointPath;

private:
  TSharedPtr<FRJointStateConfigurationClient> ServiceClient;
  TSharedPtr<rosapi::GetParam::Request> Request;
  TSharedPtr<rosapi::GetParam::Response> Response;
};

// class FROSJointControllerConfigurationClient final : public FROSBridgeSrvClient
// {
// public:
//   FROSJointControllerConfigurationClient(TMap<FString, float> *OutJointNames, const FString &InName, const FString &InType);

//   void Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;

// private:
//   TMap<FString, float> *JointNames;
// };

// class FROSJointLimitControllerConfigurationClient final : public FROSBridgeSrvClient
// {
// public:
//   FROSJointLimitControllerConfigurationClient(TMap<FString, float> *OutJointNames, AActor *InModel, const FString &InName, const FString &InType);

//   void Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;

// private:
//   TMap<FString, float> *JointNames;
//   ARModel *Model;
// };