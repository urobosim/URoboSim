#pragma once

#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvClient.h"
// #include "ROSCommunication/RROSClientImpl.h"
// clang-format off
#include "RServiceClient.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URServiceClient : public UObject
{
  GENERATED_BODY()

public:
  virtual void Init(UObject *InControllerComp){}
  virtual void Init(UObject *InControllerComp, TArray<FString> *OutArray){}
  virtual void Init(UObject *InControllerComp, TArray<FString> *OutArray, TSharedPtr<FROSBridgeHandler> InHandler);
  virtual void Init(UObject *InControllerComp, TSharedPtr<FROSBridgeHandler> InHandler);

public:
  virtual void CallService(){}
  virtual void SetParameters(float InSimTime, FJointState InParameters, FTransform InRobotPose){}

protected:
  TSharedPtr<FROSBridgeHandler> Handler;
};

// UCLASS()
// class UROBOSIM_API URJointControllerConfigurationClient : public URServiceClient
// {
//   GENERATED_BODY()

// public:
//   URJointControllerConfigurationClient();

//   UPROPERTY()
//   URControllerComponent *ControllerComp;

//   UPROPERTY(EditAnywhere)
//   FString JointParamTopic;

//   UPROPERTY(EditAnywhere)
//   FString LimitParamTopic;

//   virtual void CallService();

//   virtual void Init(UObject *InControllerComp) override;
//   virtual void CreateClient();

// private:
//   TSharedPtr<FROSJointControllerConfigurationClient> JointServiceClient;
//   TSharedPtr<FROSJointLimitControllerConfigurationClient> JointLimitServiceClient;
//   TSharedPtr<rosapi::GetParam::Request> JointRequest;
//   TSharedPtr<rosapi::GetParam::Response> JointResponse;

//   TSharedPtr<rosapi::GetParam::Request> LimitRequest;
//   TSharedPtr<rosapi::GetParam::Response> LimitResponse;
// };