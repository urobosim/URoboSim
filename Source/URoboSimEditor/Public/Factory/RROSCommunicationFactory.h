// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

// #include "UnrealEd.h"
#include "ActorFactories/ActorFactory.h"
#include "ROSCommunication/RROSCommunicationDataAsset.h"
// clang-format off
#include "RROSCommunicationFactory.generated.h"
// clang-format on

UCLASS()
class UROBOSIMEDITOR_API URROSCommunicationFactory : public UActorFactory
{
	GENERATED_BODY()

private:
	URROSCommunicationFactory(const FObjectInitializer &ObjectInitializer);

	virtual bool CanCreateActorFrom(const FAssetData &AssetData, FText &OutErrorMsg) override;

	/** Initialize NewActorClass if necessary, and return default actor for that class. */
	virtual AActor *GetDefaultActor(const FAssetData &AssetData) override;

	/** Spawns the robot */
	//virtual AActor *SpawnActor(UObject *Asset, ULevel *InLevel, const FTransform &Transform, EObjectFlags ObjectFlags, const FName Name) override;
	virtual AActor* SpawnActor(UObject* InAsset, ULevel* InLevel, const FTransform& InTransform, const FActorSpawnParameters& InSpawnParams) override;

	bool bDrag;
};
