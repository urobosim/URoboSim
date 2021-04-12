// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Physics/RModel.h"
#include "ROSBridgeHandler.h"
#include "RSubscriberParameter.h"
// clang-format off
#include "RSubscriber.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URSubscriber : public UObject
{
  GENERATED_BODY()

public:
  void Init(const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InTopic = TEXT(""));

  void Init(const FString &WebsocketIPAddr, const uint32 &WebsocketPort, const FString &InTopic = TEXT(""));

  void Tick();

  void DeInit();

public:
  ARModel *GetOwner() const { return Owner; }

  void SetOwner(UObject *InOwner){ Owner = Cast<ARModel>(InOwner); }

  virtual void SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters);

public:
  UPROPERTY(EditAnywhere)
  FString Topic;

protected:
  void Init(const FString &InTopic);

  void AddSubscriber();

  virtual void Init();

  virtual void CreateSubscriber(){}

protected:
  UPROPERTY(VisibleAnywhere)
  FString MessageType;
  
  TSharedPtr<FROSBridgeSubscriber> Subscriber;

  TSharedPtr<FROSBridgeHandler> Handler;

private:
  ARModel *Owner;
};
