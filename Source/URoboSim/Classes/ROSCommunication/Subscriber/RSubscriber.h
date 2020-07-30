// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSCommunication/Subscriber/RROSSubscriberCallback.h"
#include "RSubscriber.generated.h"

class URControllerComponent;
class ARModel;

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URSubscriber: public UObject
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere)
      FString Topic;

    UPROPERTY()
      ARModel* Model;

    UPROPERTY()
      URControllerComponent* ControllerComponent;

    virtual void Init(UObject* InModel, TSharedPtr<FROSBridgeHandler> InHandler, FString InRosTopic="");

    virtual void Init(UObject* InModel){};

    TSharedPtr<FROSBridgeSubscriber> Subscriber;

    UPROPERTY(EditAnywhere)
      FString ControllerName;
 protected:
    virtual void Init(FString RosTopic);


    UPROPERTY()
      FString MessageType;

 private:

    virtual void SetMessageType(){};
    virtual void CreateSubscriber(){};

};
