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
  virtual void AddJoint(URJoint *&Joint) { Joints.Add(Joint); }

  virtual void AddLink(URLink *&Link) { Links.Add(Link); }

  virtual void AddPlugin(UActorComponent *Plugin) { Plugins.Add(Plugin); }

  virtual TArray<URJoint *> GetJoints() const { return Joints; }

  virtual URJoint *GetJoint(const FString &JointName) const
  {
    return *Joints.FindByPredicate([&](URJoint *Joint) { return Joint->GetName().Equals(JointName); });
  }

  virtual TArray<URLink *> GetLinks() const { return Links; }

  virtual UActorComponent *GetPlugin(const FString &PluginName) const
  {
    return *Plugins.FindByPredicate([&](UActorComponent *Plugin) { return Plugin->GetName().Equals(PluginName); });
  }

  // virtual const TArray<FJointState> GetJointState() const;
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
  TArray<UActorComponent *> Plugins;

  // UPROPERTY()
  // TArray<URGraspComponent *> Grippers;
};