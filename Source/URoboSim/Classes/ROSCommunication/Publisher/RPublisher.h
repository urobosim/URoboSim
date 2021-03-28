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
  virtual void Init(UObject *InOwner, const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InTopic = TEXT(""));

  virtual void Init(UObject *InOwner, const FString &WebsocketIPAddr, const uint32 &WebsocketPort, const FString &InTopic = TEXT(""));

  virtual void DeInit()
  {
    if (Handler.IsValid())
    {
      Handler->Disconnect();
    }
  }

  virtual void Tick();

public:
  virtual ARModel *GetOwner() const { return Owner; }

  virtual void Publish() {}

protected:
  virtual void Init(UObject *InOwner, const FString &InTopic);

  virtual void Init() {}

  virtual void SetOwner(UObject *&InOwner) { Owner = Cast<ARModel>(InOwner); }

  virtual void CreatePublisher();

protected:
  TSharedPtr<FROSBridgePublisher> Publisher;

  UPROPERTY(EditAnywhere)
  FString Topic;

  UPROPERTY()
  FString MessageType;

  TSharedPtr<FROSBridgeHandler> Handler;

private:
  UPROPERTY()
  ARModel *Owner;
};