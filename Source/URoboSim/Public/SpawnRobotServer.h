// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "CoreMinimal.h"
#include "ROSBridgeSrvServer.h"
#include "modul_spawner_msgs/srv/SpawnRobot.h"
#include "srv/ConfigureJointStatePublisher.h"
//#include "RWCManager.h"
//delete RobotFactory?
#include "ROSCommunication/Service/Client/RServiceClient.h"
#include "ROSCommunication/Service/Server/RServiceServer.h"
#include "SDF/SDFDataAsset.h"
#include "SpawnRobotServer.generated.h"

/**
 * This Service should be able to spawn Robot
 */

class URobotManager;

UCLASS()
class UROBOSIM_API USDFClient final : public UServiceClient
{
	GENERATED_BODY()

public:
	USDFClient()
	{
		ServiceName = TEXT("rosapi/get_param");
		ServiceType = TEXT("rosapi/GetParam");
	};

public:
	void CallService() override;

protected:
	void CreateServiceClient() override;

public:
	UPROPERTY(EditAnywhere)
	FString RobotDescription = TEXT("/robot_description_sdf");

	UPROPERTY()
	FString SDFFile;
	
	UPROPERTY()
	FString Id;
	
	UPROPERTY()
	URobotManager* RobotManager;
private:
	TSharedPtr<class FSDFParamClient> SDFParamClient;

	TSharedPtr<rosapi::GetParam::Request> SDFRequest;

	TSharedPtr<rosapi::GetParam::Response> SDFResponse;
};

class FSDFParamClient final : public FROSBridgeSrvClient
{
public:
	FSDFParamClient(URobotManager* OutRobotManager, const FString& InName, const FString& InType);

	void Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;

private:
	URobotManager* RobotManager;
};

UCLASS()
class UROBOSIM_API USpawnRobotServer final : public UServiceServer
{
	GENERATED_BODY()

public:
	USpawnRobotServer()
	{
		Name = TEXT("RobotSpawnServer");
		Type = TEXT("SpawnRobot");
	};

protected:
	virtual void CreateServiceServer() override;

public:
	// UPROPERTY(EditAnywhere)
	// FString RobotDescription;

	UPROPERTY()
	FString SDFFile;

	UPROPERTY(EditAnywhere)
	USDFClient* SDFClient;

	UPROPERTY()
	URobotManager* Controller;
private:
	// TSharedPtr<class FSDFParamClient> SDFParamClient;
};

class FROSSpawnRobotServer : public FROSBridgeSrvServer
{
protected:
	FROSSpawnRobotServer()
	{
	};

	FTimerHandle MyTimerHandle;

	UWorld* World;

	FThreadSafeBool ServiceSuccess;


private:
	AActor* SpawnRobotFromAsset(USDFDataAsset* InDataAsset, FString InId);

	UPROPERTY(EditAnywhere)
	FString RobotDescription;

	UPROPERTY()
	USDFClient* SDFClient;
public:
	//URWCManager* Controller;
	URobotManager* Controller;


	FROSSpawnRobotServer(FString Namespace, FString Name, UWorld* InWorld, URobotManager* InController,
	                     USDFClient* InSDFClient) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("modul_spawner_msgs/SpawnRobot"))
	{
		Controller = InController;
		World = InWorld;
		SDFClient = InSDFClient;
	}

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
