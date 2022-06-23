// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "SpawnRobotServer.h"
#include "RobotManagerActor.generated.h"


UCLASS()
class UROBOSIM_API URobotManager : public UObject
{
	GENERATED_BODY()
public:
	URobotManager();
	
	~URobotManager()
	{
	}

	UPROPERTY(EditAnywhere)
	bool bUseGlobalIP = true;

	// ROSBridge server IP
	UPROPERTY(EditAnywhere)
	FString ServerIP;

	// ROSBridge server PORT
	UPROPERTY(EditAnywhere, meta = (ClampMin = 0, ClampMax = 65535))
	int32 ServerPORT;

	UPROPERTY()
	USDFClient* SDFClient;

	UPROPERTY(EditAnywhere)
	USpawnRobotServer* SpawnRobotServer;

	UFUNCTION()
	void ParseSDF();
	
	// // ROSBridge handler for ROS connection
	// TSharedPtr<FROSBridgeHandler> ROSBridgeHandler;
	//
	// // ROSPublisher for publishing TF
	// TSharedPtr<FROSBridgeSrvServer> Server;
	//
	virtual void Init(FString DefaultNamespace, UWorld* InWorld = nullptr);
	virtual void EndPlay(const EEndPlayReason::Type Reason);

private:
	FString Namespace;
};

UCLASS()
class UROBOSIM_API ARobotManagerActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARobotManagerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type Reason) override;

	UPROPERTY(EditAnywhere)
	URobotManager* RobotManager;
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
