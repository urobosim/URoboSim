#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
#include "world_control_msgs/srv/ResetLevel.h"


class FROSResetLevelServer : public FROSBridgeSrvServer
{
protected:

	FROSResetLevelServer()
	{
          UE_LOG(LogTemp, Error, TEXT("ResetLevelserver created"));
	};

  UWorld* World;
  UObject* Owner;
  FThreadSafeBool bAllSuccessfull;
private:

public:

  FROSResetLevelServer(FString Namespace, FString Name, UWorld* InWorld, UObject* InOwner) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/ResetLevel"))
	{
          World = InWorld;
          Owner = InOwner;
	}


	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
