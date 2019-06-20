#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "PR2UnrealSimulator/RegisterRobot.h"
#include "PR2UnrealSimulator/UpdateWorld.h"
#include "srv/ConfigureJointStatePublisher.h"
#include "ROSBridgeSrvClient.h"

// class URControllerManagerComponent;
// class URROSRobotRegistrationClient;
class URControllerComponent;
class URJointStatePublisher;

// class FROSRobotRegistrationClient final : public FROSBridgeSrvClient
// {
// public:
// 	FROSRobotRegistrationClient(URROSRobotRegistrationClient* InClient, const FString& InName, const FString& InType):
// 	FROSBridgeSrvClient(InName, InType)
// 	{
// 		Client = InClient;
// 	}
//
// 	void Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;
// private:
// 		URROSRobotRegistrationClient* Client;
// };
//
// class FROSUpdateWorldClient final : public FROSBridgeSrvClient
// {
// public:
// 	FROSUpdateWorldClient(UObject* InModel, const FString& InName, const FString& InType);
//
// 	void Callback( TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;
// private:
// 	URControllerComponent* ControllerComponent;
// 	URBaseController* BaseController;
// };

class FROSJointStateConfigurationClient final : public FROSBridgeSrvClient
{
public:
	FROSJointStateConfigurationClient(TArray<FString>* OutJointNames, const FString& InName, const FString& InType);

	void Callback( TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;
private:
	URJointStatePublisher* JSPublisher;
	TArray<FString>* JointNames;
};
