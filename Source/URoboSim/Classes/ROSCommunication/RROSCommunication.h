// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "ROSBridgeHandler.h"
#include "Physics/RModel.h"
#include "ROSUtilities.h"
// clang-format off
#include "RROSCommunication.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API UROSCommunication : public UObject
{
  GENERATED_BODY()

public:
  void Connect(const FString &WebsocketIPAddr, const uint32 &WebsocketPort);

  void Connect(const TSharedPtr<FROSBridgeHandler> &InHandler);

  void Disconnect();

public:
  virtual void Tick() {}

protected:
  virtual void Init() {}

protected:
  TSharedPtr<FROSBridgeHandler> Handler;
};
