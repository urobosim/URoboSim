// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author:Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSubscriber.h"
#include "Controller/RController.h"
#include "Controller/RJointController.h"
// #include "Physics/RModel.h"
// #include "Physics/RJoint.h"

// class URJointController;
// class URController;
// class URBaseController;
// class URHeadTrajectoryController;

/**
 *
 */
// class UROBOSIM_API FROSSubscriberCallback : public FROSBridgeSubscriber
// {
// public:
// 	FROSSubscriberCallback(const FString& InTopic, const FString& InType, ARModel* InModel);
//
// 	~FROSSubscriberCallback() override;
//
// 	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;
//
// 	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;
//
// 	ARModel* Model;
// private:
// };

class UROBOSIM_API FROSWholeBodyControllerCallback : public FROSBridgeSubscriber
{
public:
	FROSWholeBodyControllerCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSWholeBodyControllerCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URJointController* JointController;
private:
};

class UROBOSIM_API FROSActionCancelCallback : public FROSBridgeSubscriber
{
public:
	FROSActionCancelCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSActionCancelCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URController* Controller;
private:
};

class UROBOSIM_API FROSFollowTrajectoryActionCancelCallback : public FROSBridgeSubscriber
{
public:
	FROSFollowTrajectoryActionCancelCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSFollowTrajectoryActionCancelCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URJointController* JointController;
private:
};

class UROBOSIM_API FROSBaseControllerCallback : public FROSBridgeSubscriber
{
public:
	FROSBaseControllerCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSBaseControllerCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URBaseController* BaseController;
private:
};

class UROBOSIM_API FROSVelocityCommandSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FROSVelocityCommandSubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSVelocityCommandSubscriberCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URBaseController* BaseController;
private:
};

class UROBOSIM_API FROSPointHeadGoalSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FROSPointHeadGoalSubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSPointHeadGoalSubscriberCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URHeadTrajectoryController* Controller;
};

class UROBOSIM_API FROSPerceiveObjectGoalSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FROSPerceiveObjectGoalSubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSPerceiveObjectGoalSubscriberCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URCameraController* Controller;
};

class UROBOSIM_API FROSGripperCommandSubscriberCallback : public FROSBridgeSubscriber
{
public:
	FROSGripperCommandSubscriberCallback(const FString& InTopic, const FString& InType, UObject* InController);

	~FROSGripperCommandSubscriberCallback() override;

	TSharedPtr<FROSBridgeMsg> ParseMessage(TSharedPtr<FJsonObject> JsonObject) const override;

	void Callback(TSharedPtr<FROSBridgeMsg> Msg) override;

	URGripperController* Controller;
};
