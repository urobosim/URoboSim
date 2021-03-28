// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Physics/RModel.h"
#include "ROSBridgeHandler.h"
// clang-format off
#include "RSubscriber.generated.h"
// clang-format on

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URSubscriber : public UObject
{
  GENERATED_BODY()

public:
  virtual void Init(UObject *InOwner, const TSharedPtr<FROSBridgeHandler> &InHandler, const FString &InTopic = TEXT(""));

  virtual void Init(UObject *InOwner, const FString &WebsocketIPAddr, const uint32 &WebsocketPort, const FString &InTopic = TEXT(""));

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

protected:
  virtual void Init(UObject *InOwner, const FString &InTopic);

  virtual void Init(){};
  
  virtual void SetOwner(UObject *&InOwner){ Owner = Cast<ARModel>(InOwner); }
  
  virtual void CreateSubscriber(){}

  virtual void AddSubscriber();

protected:
  TSharedPtr<FROSBridgeSubscriber> Subscriber;

  UPROPERTY(EditAnywhere)
  FString Topic;

  UPROPERTY()
  FString MessageType;

  TSharedPtr<FROSBridgeHandler> Handler;

private:
  UPROPERTY()
  ARModel *Owner;
};
