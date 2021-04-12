#pragma once

#include "Physics/RModel.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvClient.h"
#include "RServiceClientParameter.h"
// clang-format off
#include "RServiceClient.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URServiceClient : public UObject
{
  GENERATED_BODY()

public:
  void Init(const TSharedPtr<FROSBridgeHandler> &InHandler);

  void Init(const FString &WebsocketIPAddr, const uint32 &WebsocketPort);

  void Tick();

  void DeInit();

public:
  ARModel *GetOwner() const { return Owner; }

  void SetOwner(UObject *InOwner){ Owner = Cast<ARModel>(InOwner); }

  virtual void SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters);

  virtual void CallService(){}

protected:
  virtual void Init();

protected:
  UPROPERTY(EditAnywhere)
  FString ServiceName;

  UPROPERTY(EditAnywhere)
  FString ServiceType;
  
  TSharedPtr<FROSBridgeHandler> Handler;

private:
  ARModel *Owner;
};