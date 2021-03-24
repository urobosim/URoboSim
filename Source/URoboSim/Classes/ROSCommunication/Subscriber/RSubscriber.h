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
  virtual void Init(UObject *InModel, TSharedPtr<FROSBridgeHandler> InHandler, FString InRosTopic = "");

  virtual void Init(UObject *InModel){};

public:
  virtual ARModel *GetOwner() const { return Owner; }

protected:
  virtual void SetMessageType(){};

  virtual void SetOwner(UObject *&InOwner){ Owner = Cast<ARModel>(InOwner); };

  virtual void CreateSubscriber();

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
