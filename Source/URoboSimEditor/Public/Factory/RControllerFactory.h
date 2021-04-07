// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "ActorFactories/ActorFactory.h"
// clang-format off
#include "RControllerFactory.generated.h"
// clang-format on

/**
 * Own ActorFactory which creates a controller from DataAsset.
 */
UCLASS()
class UROBOSIMEDITOR_API URControllerFactory : public UActorFactory
{
  GENERATED_BODY()

private:
  URControllerFactory(const FObjectInitializer &ObjectInitializer);

  /**
     * Checks if AssetData is intended for the factory.
     */
  virtual bool CanCreateActorFrom(const FAssetData &AssetData, FText &OutErrorMsg) override;

  /** Initialize NewActorClass if necessary, and return default actor for that class. */
  virtual AActor *GetDefaultActor(const FAssetData &AssetData) override;

  /** Spawns the robot */
  virtual AActor *SpawnActor(UObject *Asset, ULevel *InLevel, const FTransform &Transform, EObjectFlags ObjectFlags, const FName Name) override;
};