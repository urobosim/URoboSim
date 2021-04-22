// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ROSCommunication/RROSCommunication.h"
// clang-format off
#include "RSubscriber.generated.h"
// clang-format on

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URSubscriberParameter : public UObject
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  FString Topic;

  UPROPERTY(VisibleAnywhere)
  FString MessageType;
};

UCLASS()
class UROBOSIM_API URSubscriber : public URROSCommunication
{
  GENERATED_BODY()

public:
  void Tick() override;

public:
  virtual void SetSubscriberParameters(URSubscriberParameter *&SubscriberParameters);

public:
  UPROPERTY(EditAnywhere)
  FString Topic;

protected:
  virtual void Init() override;

  void AddSubscriber();
  
  virtual void CreateSubscriber(){}

protected:
  UPROPERTY(VisibleAnywhere)
  FString MessageType;
  
  TSharedPtr<FROSBridgeSubscriber> Subscriber;
};
