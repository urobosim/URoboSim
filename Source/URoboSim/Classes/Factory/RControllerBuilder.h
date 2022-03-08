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
  virtual void Init(const TArray<ARModel *> &InModels, const FRControllerConfiguration &InControllerConfiguration);

  virtual void Build();

protected:
  virtual URController *CreateController(ARModel *&InOwner, const TPair<FString, URControllerParameter *> ControllerParameters);

protected:
  TArray<ARModel *> Models;

  FRControllerConfiguration ControllerConfiguration;
};