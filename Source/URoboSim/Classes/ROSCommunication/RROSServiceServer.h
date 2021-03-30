#pragma once

#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
#include "ROSBridgeSrvServer.h"
#include "PR2UnrealSimulator/RegisterRobot.h"
#include "srv/SimulationCommands.h"
#include "Controller/RControllerComponent.h"
#include "std_msgs/String.h"


class FROSRobotRegistrationServer final : public FROSBridgeSrvServer
{
public:


	// TSharedPtr<FROSBridgeHandler> Handler;
	FROSRobotRegistrationServer(const FString& InName, const FString& InType):
		        FROSBridgeSrvServer(InName, InType)
	{
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;
	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest) override;
};




// class FROSGripperControllerService final : public FROSBridgeSrvServer
// {
// public:
//
// 	// TSharedPtr<FROSBridgeHandler> Handler;
// 	FROSGripperControllerService(const FString& InName, const FString& InType):
// 		        FROSBridgeSrvServer(InName, InType)
// 	{
// 	}
//
// 	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;
// 	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest) override;
// };


class FROSSimulationCommandsServer final : public FROSBridgeSrvServer
{
public:

	// TSharedPtr<FROSBridgeHandler> Handler;
	FROSSimulationCommandsServer(const FString& InName, const FString& InType, UObject* InOwner):
		        FROSBridgeSrvServer(InName, InType)
	{
		Owner = Cast<URControllerComponent>(InOwner);
	}

	URControllerComponent* Owner;

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;
	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest) override;
};
