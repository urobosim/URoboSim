#pragma once

#include "Physics/RModel.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvClient.h"
// clang-format off
#include "RServiceClient.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URServiceClient : public UObject
{
  GENERATED_BODY()

public:
  virtual void Init(UObject *InOwner, const TSharedPtr<FROSBridgeHandler> &InHandler);

  virtual void Init(UObject *InOwner, const FString &WebsocketIPAddr, const uint32 &WebsocketPort);

  virtual void Tick();

  virtual void DeInit() 
  { 
    if (Handler.IsValid())
    {
      Handler->Disconnect(); 
    }
  }

public:
  virtual ARModel *GetOwner() const { return Owner; }

  virtual void CallService(){}

protected:
  virtual void Init(UObject *InOwner);

  virtual void Init(){}

  virtual void SetOwner(UObject *&InOwner){ Owner = Cast<ARModel>(InOwner); }

protected:
  UPROPERTY(EditAnywhere)
  FString ServiceName;

  UPROPERTY(EditAnywhere)
  FString ServiceType;
  
  TSharedPtr<FROSBridgeHandler> Handler;

private:
  UPROPERTY()
  ARModel *Owner;
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