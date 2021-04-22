// Copyright 2021, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Physics/RModel.h"
#include "ROSBridgeHandler.h"
// clang-format off
#include "RROSCommunication.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URROSCommunication : public UObject
{
  GENERATED_BODY()

public:
  void Connect(const FString &WebsocketIPAddr, const uint32 &WebsocketPort);

  void Connect(const TSharedPtr<FROSBridgeHandler> &InHandler);

  void Disconnect();

  ARModel *GetOwner() const { return Owner; }

  void SetOwner(UObject *InOwner){ Owner = Cast<ARModel>(InOwner); }

  void SetOwner();

public:
  virtual void Tick() {};

protected:
  virtual void Init();

protected:
  TSharedPtr<FROSBridgeHandler> Handler;

private:
  ARModel *Owner;
};