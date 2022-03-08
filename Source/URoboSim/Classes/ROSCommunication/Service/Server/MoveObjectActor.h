#pragma once

#include "GameFramework/Actor.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
// clang-format off
#include "MoveObjectActor.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API AMoveObjectActor : public AActor
{
  GENERATED_BODY()

public:
  AMoveObjectActor()
  {
    WebsocketIPAddr = TEXT("127.0.0.1");
    WebsocketPort = 9090;
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = TG_PrePhysics;
  };

public:
  virtual void BeginPlay() override;

  virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction &ThisTickFunction) override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
  
public:
  UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
  FString WebsocketIPAddr;

  UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
  uint32 WebsocketPort;

private:
  TSharedPtr<FROSBridgeHandler> Handler;

  TSharedPtr<FROSBridgeSrvServer> Server;
};

class FRMoveObjectServerCallback : public FROSBridgeSrvServer
{
public:
  FRMoveObjectServerCallback(FString Namespace, FString Name, UWorld *InWorld, UObject *InOwner) : FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("world_control_msgs/MoveObject"))
	{
		World = InWorld;
		Owner = InOwner;
	}

  TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

private:
	UWorld *World;

	UObject *Owner;

	FThreadSafeBool bAllSuccessfull;
};
