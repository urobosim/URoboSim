#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "CheckVisibilityService.generated.h"

UCLASS()
class UROBOSIM_API UCheckVisibilityService : public UObject
{
    GENERATED_BODY()

public:
    virtual void Init();

    UCheckVisibilityService()
      {
        WebsocketIPAddr = TEXT("127.0.0.1");
	WebsocketPort = 9090;
      };
    TSharedPtr<FROSBridgeHandler> Handler;

    TSharedPtr<FROSBridgeSrvServer> Server;

    UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
    FString WebsocketIPAddr;

    UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
    uint32 WebsocketPort;

};
