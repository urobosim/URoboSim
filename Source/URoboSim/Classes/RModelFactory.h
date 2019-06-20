// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "UnrealEd.h"
#include "SDF/SDFDataAsset.h"
#include "RModelFactory.generated.h"

class ARModel;
// class SDFAssetData;
// struct FSDFLink;
// struct FSDFJoint;

/**
 * Own ActorFactory which creates a robot from SDFData Asset.
 */
UCLASS()
class UROBOSIM_API URModelFactory : public UActorFactory
{
    GENERATED_BODY()

private:
    URModelFactory(const FObjectInitializer& ObjectInitializer);

#if WITH_EDITOR

    /**
     * Checks if AssetData is intended for the factory.
     * Checks if an RRobot can be built by AssetData.
     */
    virtual bool CanCreateActorFrom(const FAssetData& AssetData, FText& OutErrorMsg) override;

    /** Initialize NewActorClass if necessary, and return default actor for that class. */
    virtual AActor* GetDefaultActor(const FAssetData& AssetData) override;

    /** Spawns the robot */
    virtual AActor* SpawnActor(UObject* Asset, ULevel* InLevel, const FTransform& Transform, EObjectFlags ObjectFlags, const FName Name) override;

    virtual void CreateModels(ARModel* OutModel, USDFDataAsset* InSDFData);


#endif
};
