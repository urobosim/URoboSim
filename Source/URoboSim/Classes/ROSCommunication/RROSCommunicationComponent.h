// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "ROSCommunication/RROSCommunication.h"
#include "RPlugin.h"
// clang-format off
#include "RROSCommunicationComponent.generated.h"
// clang-format on

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URROSCommunicationComponent : public URPluginComponent
{
  GENERATED_BODY()

public:
  URROSCommunicationComponent();

  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

protected:
  virtual void BeginPlay() override;
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
  UPROPERTY(EditAnywhere)
  FRROSCommunicationContainer ROSCommunication;
};