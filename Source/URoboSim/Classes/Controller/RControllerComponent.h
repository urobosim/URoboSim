// Copyright 2017, Institute for Artificial Intelligence - University of Bremen

#pragma once

#include "Controller/RController.h"
#include "RPlugin.h"
// clang-format off
#include "RControllerComponent.generated.h"
// clang-format on

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URControllerComponent : public URPluginComponent
{
  GENERATED_BODY()

public:
  URControllerComponent();

public:
  virtual void BeginPlay() override;
  
public:
  virtual void Init();

  virtual void AddController(URController *Controller) { Controllers.Add(Controller); }

  virtual URController *GetController(const FString &ControllerName);

  virtual TArray<URController *> GetControllers() const { return Controllers; }

protected:
  // virtual URController *ControllerList(FString ControllerName);

  // virtual void SetJointVelocities(TArray<FString> InJointNames, TArray<float> InJointVelocities);

  UPROPERTY(BlueprintReadWrite, Instanced, EditAnywhere, export, noclear)
  TArray<URController *> Controllers;
};