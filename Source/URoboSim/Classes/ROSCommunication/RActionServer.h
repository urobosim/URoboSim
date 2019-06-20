#pragma once

#include "ROSBridgeHandler.h"
#include "ROSCommunication/RPublisher.h"
#include "ROSCommunication/RSubscriber.h"
#include "RActionServer.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URActionServer : public UObject
{
    GENERATED_BODY()
public:

	URActionServer(){};

    virtual void Init(TSharedPtr<FROSBridgeHandler> InHandler, UObject* InOwner, FString InActionName= "");
    virtual void Init(FString InHostIp, uint32 InPort, UObject* InOwner, FString InActionName= "");

	virtual void Tick();

    UPROPERTY(EditAnywhere)
    FString ControllerName;
protected:

	UPROPERTY(EditAnywhere)
	URPublisher* FeedbackPublisher;

	UPROPERTY(EditAnywhere)
	URPublisher* StatusPublisher;

	UPROPERTY(EditAnywhere)
	URPublisher* ResultPublisher;

	UPROPERTY(EditAnywhere)
	URSubscriber* GoalSubscriber;

	UPROPERTY(EditAnywhere)
	URSubscriber* CancelSubscriber;

	TSharedPtr<FROSBridgeHandler> Handler;

	virtual void Init();

	UPROPERTY()
	UObject* Owner;

	UPROPERTY(EditAnywhere)
	FString ActionName;
};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URFollowTrajectoryActionServer : public URActionServer
{
    GENERATED_BODY()
public:
		URFollowTrajectoryActionServer();
};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URPointHeadActionServer : public URActionServer
{
    GENERATED_BODY()
public:
		URPointHeadActionServer();
};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URGripperCommandActionServer : public URActionServer
{
    GENERATED_BODY()
public:
		URGripperCommandActionServer();

};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URPerceiveObjectActionServer : public URActionServer
{
    GENERATED_BODY()
public:
		URPerceiveObjectActionServer();

};
