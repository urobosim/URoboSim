// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "RPluginComponent.h"
#include "RROSCommunication.h"
#include "RROSCommunicationComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API URROSCommunicationComponent : public URPluginComponent
{
    GENERATED_BODY()
public:
    URROSCommunicationComponent();


    UPROPERTY(EditAnywhere)
    FRROSCommunicationContainer ROSCommunication;

    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
