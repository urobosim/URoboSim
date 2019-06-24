#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "MoveObjectActor.generated.h"




UCLASS()
class UROBOSIM_API AMoveObjectActor : public AActor
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    AMoveObjectActor()
      {
        WebsocketIPAddr = TEXT("127.0.0.1");
	WebsocketPort = 9090;
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = TG_PrePhysics;
      };
    TSharedPtr<FROSBridgeHandler> Handler;

    TSharedPtr<FROSBridgeSrvServer> Server;

    UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
    FString WebsocketIPAddr;

    UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
    uint32 WebsocketPort;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;
};
