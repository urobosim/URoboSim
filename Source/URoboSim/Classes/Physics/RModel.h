// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "GameFramework/Actor.h"
// #include "RGraspComponent.h"
#include "Physics/RJoint.h"
#include "Physics/RLink.h"
// clang-format off
#include "RModel.generated.h"
// clang-format on

class URPluginComponent;

USTRUCT()
struct FEnableGravity
{
  GENERATED_BODY()

public:
  FEnableGravity() : bBase(true), bLinks(false) {}

  UPROPERTY(EditAnywhere)
  bool bBase;

  UPROPERTY(EditAnywhere)
  bool bLinks;
};

UCLASS()
class UROBOSIM_API ARModel : public AActor
{
  GENERATED_BODY()

public:
  // Sets default values
  ARModel();

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  void Init();

  void AddJoint(URJoint *&Joint) { Joints.Add(Joint); }

  void AddLink(URLink *&Link) { Links.Add(Link); }

  bool AddPlugin(URPluginComponent *Plugin);

  TArray<URJoint *> GetJoints() const { return Joints; }

  URJoint *GetJoint(const FString &JointName) const;

  TArray<URLink *> GetLinks() const { return Links; }

  URLink *GetBaseLink() const;

  URPluginComponent *GetPlugin(const FString &PluginName) const;

public:
  UPROPERTY(EditAnywhere)
  FEnableGravity EnableGravity;

  UPROPERTY(EditAnywhere)
  bool bSimulatePhysics;

protected:
  UPROPERTY(VisibleAnywhere)
  TArray<URJoint *> Joints;

  UPROPERTY(VisibleAnywhere)
  TArray<URLink *> Links;

  UPROPERTY(EditAnywhere)
  TArray<URPluginComponent *> Plugins;

  // TArray<URGraspComponent *> Grippers;
private:
  bool bModelInitialized = false;
};