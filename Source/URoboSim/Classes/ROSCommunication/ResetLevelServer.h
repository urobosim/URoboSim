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
  UGameInstance* Owner;
  FThreadSafeBool bAllSuccessfull;
private:

public:

  FROSResetLevelServer(FString Namespace, FString Name, UObject* InOwner) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/ResetLevel"))
	{
          World = nullptr;
          Owner = Cast<UGameInstance>(InOwner);
	}


	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
