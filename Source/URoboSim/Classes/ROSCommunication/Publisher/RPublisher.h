#pragma once

#include "Physics/RModel.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
// clang-format off
#include "RPublisher.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URPublisher : public UObject
{
  GENERATED_BODY()

public:
  virtual void Init(UObject *InOwner, const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InTopic);
  virtual void Init(UObject *InOwner, const FString &InHostIp, const uint32 &InPort);
  virtual void DeInit();

public:
  virtual ARModel *GetOwner() const { return Owner; }

  virtual void Publish(){};

protected:
  virtual void SetMessageType(){};
  virtual void SetOwner(UObject *&InOwner){ Owner = Cast<ARModel>(InOwner); };
  virtual void CreatePublisher();

protected:
  TSharedPtr<FROSBridgePublisher> Publisher;

  int Seq;

  UPROPERTY()
  FString ControllerName;

  UPROPERTY(EditAnywhere)
  FString Topic;

  UPROPERTY()
  FString MessageType;

  TSharedPtr<FROSBridgeHandler> Handler;

private:
  UPROPERTY()
  ARModel *Owner;
};