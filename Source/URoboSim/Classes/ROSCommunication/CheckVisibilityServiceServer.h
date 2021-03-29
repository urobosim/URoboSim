#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Physics/RModel.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
#include "srv/CheckVisibility.h"


class FROSCheckVisebilityServer : public FROSBridgeSrvServer
{
protected:

	FROSCheckVisebilityServer()
	{
	};

  UWorld* World;
  ARModel* Owner;
  FThreadSafeBool bAllSuccessfull;
private:

public:

  FROSCheckVisebilityServer(FString Namespace, FString Name, UWorld* InWorld, UObject* InOwner) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("urobosim_msgs/CheckVisibility"))
	{
          World = InWorld;
          Owner = Cast<ARModel>(InOwner);
	}


	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
