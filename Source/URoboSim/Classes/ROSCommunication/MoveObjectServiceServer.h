#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
#include "world_control_msgs/srv/MoveObject.h"


class FROSMoveObjectServer : public FROSBridgeSrvServer
{
protected:

	FROSMoveObjectServer()
	{
	};

  UWorld* World;
  UObject* Owner;
  FThreadSafeBool bAllSuccessfull;
private:

public:

  FROSMoveObjectServer(FString Namespace, FString Name, UWorld* InWorld, UObject* InOwner) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/MoveObject"))
	{
          World = InWorld;
          Owner = InOwner;
	}


	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
