#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "ROSCommunication/RROSService.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
#include "Physics/RModel.h"
#include "srv/SpawnRobotGhost.h"
#include "SpawnRobotGhost.generated.h"


class FSpawnRobotGhostServer : public FROSBridgeSrvServer
{
protected:

	FSpawnRobotGhostServer()
	{
	};

  UWorld* World;
  UObject* Owner;
  FThreadSafeBool bAllSuccessfull;
private:

public:

  FSpawnRobotGhostServer(FString Namespace, FString Name, UWorld* InWorld, UObject* InOwner) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("urobosim_msgs/SpawnRobotGhost"))
	{
          World = InWorld;
          Owner = InOwner;
	}


	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

};

UCLASS()
class UROBOSIM_API USpawnRobotGhostService : public URROSService
{
GENERATED_BODY()
public:


protected:
	UPROPERTY()
	ARModel* Owner;

	virtual void SetOwner(UObject* InOwner);
	virtual void SetType();
	virtual void CreateServiceServer();

};

	/* ROSHandler->AddServiceServer(MakeShareable<FROSResetLevelServer>(new FROSResetLevelServer(FString("UnrealSim"), TEXT("reset_level"),  this))); */
