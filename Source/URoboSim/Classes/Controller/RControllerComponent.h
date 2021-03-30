// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Controller/RController.h"
#include "RPlugin.h"
// clang-format off
#include "RControllerComponent.generated.h"
// clang-format on

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URControllerComponent final : public URPluginComponent
{
  GENERATED_BODY()

public:
  URControllerComponent();
  
public:
  void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

  void Init() override;

  void AddController(URController *Controller) { Controllers.Add(Controller); }

  URController *GetController(const FString &ControllerName);

  TArray<URController *> GetControllers() const { return Controllers; }

protected:
  UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
  TArray<URController *> Controllers;
};