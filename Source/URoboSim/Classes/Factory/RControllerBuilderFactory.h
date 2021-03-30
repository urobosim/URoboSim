// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann (neumann.michael@uni-bremen.de)

#pragma once

#include "CoreMinimal.h"
#include "Physics/RModel.h"
#include "Controller/RControllerDataAsset.h"
#include "Controller/RControllerComponent.h"
#include "RControllerBuilderFactory.generated.h"

UCLASS()
class UROBOSIM_API URControllerBuilder : public UObject
{
  GENERATED_BODY()

public:

  virtual void Init(TArray<ARModel*> InModels, const URControllerConfiguration* InControllerConfiguration);


  virtual void Build();
  virtual void ConfigureController(){};

protected:

  UPROPERTY()
  TArray<ARModel*> Models;

  UPROPERTY()
  TArray<URControllerComponent*> ControllerComponents;

  UPROPERTY()
  const URControllerConfiguration* ControllerConfiguration;

  virtual URController* CreateController(UObject* InOwner){return nullptr;};

};

UCLASS()
class UROBOSIM_API URGripperControllerBuilder : public URControllerBuilder
{
  GENERATED_BODY()

  public:
  virtual void ConfigureController() override;

  protected:

  virtual URController* CreateController(UObject* InOwner) override;
};

UCLASS()
class UROBOSIM_API URBaseControllerBuilder : public URControllerBuilder
{
  GENERATED_BODY()

  public:
  virtual void ConfigureController() override;

  protected:

  virtual URController* CreateController(UObject* InOwner) override;
};

UCLASS()
class UROBOSIM_API URJointControllerBuilder : public URControllerBuilder
{
  GENERATED_BODY()

  public:
  virtual void ConfigureController() override;

  protected:

  virtual URController* CreateController(UObject* InOwner) override;
};

UCLASS()
class UROBOSIM_API URHeadControllerBuilder : public URControllerBuilder
{
  GENERATED_BODY()

  public:
  virtual void ConfigureController() override;

  protected:

  virtual URController* CreateController(UObject* InOwner) override;
};

UCLASS()
class UROBOSIM_API URCameraControllerBuilder : public URControllerBuilder
{
  GENERATED_BODY()

  public:
  virtual void ConfigureController() override;

  protected:

  virtual URController* CreateController(UObject* InOwner) override;
};

UCLASS()
class UROBOSIM_API URControllerBuilderFactory : public UObject
{
  GENERATED_BODY()
 public:

  URControllerBuilder* CreateBuilder(TArray<ARModel*> InModel, const URControllerConfiguration* InControllerConfiguration);

protected:

  virtual void CreateBuilder();

  UPROPERTY()
    const URControllerConfiguration* ControllerConfiguration;

  UPROPERTY()
    URControllerBuilder* ControllerBuilder;

};
