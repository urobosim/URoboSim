#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Physics/RModel.h"
#include "ROSBridgeSrvServer.h"
#include "ROSBridgeHandler.h"
#include "RGBDCamera.h"
#include "Controller/RControllerComponent.h"
#include "srv/CheckVisibility.h"


class FROSCheckVisebilityServer : public FROSBridgeSrvServer
{
protected:

	FROSCheckVisebilityServer()
	{
	};

  UWorld* World;
  ARModel* Owner;
  int32 PlayerIndex;
  FThreadSafeBool bAllSuccessfull;
private:

public:

  FROSCheckVisebilityServer(FString Name, FString Type, UWorld* InWorld, UObject* InOwner) :
		FROSBridgeSrvServer(Name, Type)
	{
          World = InWorld;
          Owner = Cast<ARModel>(InOwner);
          if(Owner)
            {
              URControllerComponent* ControllerComp = Cast<URControllerComponent>(Owner->Plugins["ControllerComponent"]);
              if(ControllerComp)
                {
                  URCameraController* CameraController = Cast<URCameraController>(ControllerComp->ControllerList("CameraController"));
                  if(CameraController)
                    {
                      ARGBDCamera* Camera = CameraController->Camera;
                      if(Camera)
                        {
                          PlayerIndex = Camera->GetAutoActivatePlayerIndex();
                        }
                    }
                }
            }
	}


	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};
