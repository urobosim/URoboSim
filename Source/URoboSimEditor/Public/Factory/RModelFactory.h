// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "ActorFactories/ActorFactory.h"
#include "Physics/RModel.h"
#include "SDF/SDFDataAsset.h"
// clang-format off
#include "RModelFactory.generated.h"
// clang-format on

/**
 * Own ActorFactory which creates a robot from SDFData Asset.
 */
UCLASS()
class UROBOSIMEDITOR_API URModelFactory : public UActorFactory
{
	GENERATED_BODY()

private:
	URModelFactory(const FObjectInitializer &ObjectInitializer);

	/**
	 * Checks if AssetData is intended for the factory.
	 * Checks if a ARModel can be built by AssetData.
	 */
	virtual bool CanCreateActorFrom(const FAssetData &AssetData, FText &OutErrorMsg) override;

	/** Initialize NewActorClass if necessary, and return default actor for that class. */
	virtual AActor *GetDefaultActor(const FAssetData &AssetData) override;

	/** Spawns the robot */
	virtual AActor *SpawnActor(UObject *Asset, ULevel *InLevel, const FTransform &Transform, EObjectFlags ObjectFlags, const FName Name) override;

	/** Subclasses may implement this to modify the actor after it has been spawned 
	    IMPORTANT: If you override this, you should usually also override PostCreateBlueprint()! */
	virtual void PostSpawnActor( UObject* Asset, AActor* NewActor ) override;

	/** Override this in derived factory classes if needed.  This is called after a blueprint is created by this factory to
	    update the blueprint's CDO properties with state from the asset for this factory.
		IMPORTANT: If you override this, you should usually also override PostSpawnActor()! */
	virtual void PostCreateBlueprint( UObject* Asset, AActor* CDO ) override;
};
