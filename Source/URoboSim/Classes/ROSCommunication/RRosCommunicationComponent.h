// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Controller/RController.h"
#include "ROSCommunication/RRosComunication.h"
#include "RRosCommunicationComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API URROSCommunicationComponent : public UActorComponent
{
    GENERATED_BODY()
public:
	URROSCommunicationComponent();


	UPROPERTY(EditAnywhere)
	FRROSComunicationContainer RosComunication;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
