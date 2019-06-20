// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
// #include "ResetLevelServer.h"
#include "RWorldControl.generated.h"

UCLASS()
class AWorldController : public AActor
{
  GENERATED_BODY()
public:

  TSharedPtr<FROSBridgeHandler> Handler;
  TSharedPtr<FROSBridgeSrvServer> Server;
  UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
  FString WebsocketIPAddr;

  UPROPERTY(EditAnywhere, Category = "ROS Bridge Robot")
  uint32 WebsocketPort;

  AWorldController();
  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
  virtual void TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction) override;

  virtual void ResetLevel();
};
