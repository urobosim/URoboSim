// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann (neumann.michael@uni-bremen.de)

#pragma once

#include "Controller/RControllerComponent.h"
#include "Controller/RControllerDataAsset.h"
// clang-format off
#include "RControllerBuilder.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URControllerBuilder : public UObject
{
  GENERATED_BODY()

public:
  virtual void Init(const TArray<ARModel *> &InModels, const URControllerConfiguration *&InControllerConfiguration);

  virtual void Build();

protected:
  virtual URController *CreateController(ARModel *&InOwner) { return nullptr; }

  virtual void Configure(){}

protected:
  UPROPERTY()
  TArray<ARModel *> Models;

  UPROPERTY()
  const URControllerConfiguration *ControllerConfiguration;
};

UCLASS()
class UROBOSIM_API URBaseControllerBuilder : public URControllerBuilder
{
  GENERATED_BODY()

protected:
  virtual URController *CreateController(ARModel *&InOwner) override;

  virtual void Configure() override;
};

UCLASS()
class UROBOSIM_API URJointControllerBuilder : public URControllerBuilder
{
  GENERATED_BODY()

protected:
  virtual URController *CreateController(ARModel *&InOwner) override;

  virtual void Configure() override;
};

URControllerBuilder *CreateBuilder(const TArray<ARModel *> InModels, const URControllerConfiguration *&InControllerConfiguration);