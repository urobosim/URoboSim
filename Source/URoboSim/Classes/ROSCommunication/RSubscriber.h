// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSCommunication/RROSJointStateSubscriberCallback.h"
#include "ROSCommunication/RROSSubscriberCallback.h"
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

    // static URSubscriber* Init(FString Type, UObject* Owner, FString RosTopic);
	virtual void Init(UObject* InModel){};

    TSharedPtr<FROSBridgeSubscriber> Subscriber;

    UPROPERTY()
    FString ControllerName;
protected:
    virtual void Init(FString RosTopic);


	UPROPERTY()
    FString MessageType;
private:

	virtual void SetMessageType(){};
	virtual void CreateSubscriber(){};

};

UCLASS()
class UROBOSIM_API URJointStateSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};

UCLASS()
class UROBOSIM_API URFollowJointTrajectoryActionGoalSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};

UCLASS()
class UROBOSIM_API URPR2GripperCommandActionGoalSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};

UCLASS()
class UROBOSIM_API URPR2HeadActionGoalSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};

UCLASS()
class UROBOSIM_API URActionCancelSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};

UCLASS()
class UROBOSIM_API URVelocityCommandSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};

UCLASS()
class UROBOSIM_API URPerceiveObjectActionGoalSubscriber: public URSubscriber
{
    GENERATED_BODY()
public:

	virtual void SetMessageType();
	virtual void CreateSubscriber();

};
