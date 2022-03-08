// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "GameFramework/Actor.h"
#include "Physics/RJoint.h"
#include "Physics/RLink.h"
#include "RGraspComponent.h"
// clang-format off
#include "RModel.generated.h"
// clang-format on

class URPluginComponent;

UCLASS()
class UROBOSIM_API ARModel : public AActor
{
  GENERATED_BODY()

public:
  // Sets default values for this actor's properties
  ARModel();

public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

public:
  void AddJoint(URJoint *Joint);

  TArray<URJoint *> GetJoints() const;

  URJoint *GetJoint(const FString &JointName) const;

  TArray<URLink *> GetLinks() const;

  URLink *GetLink(const FString &LinkName) const;

  void AddLink(URLink *Link);

  bool AddPlugin(URPluginComponent *InPlugin); 

  URPluginComponent *GetPlugin(const FString &PluginName) const;

  class URController *GetController(const FString &ControllerName) const;

public:
  UPROPERTY(EditAnywhere)
  TMap<FString, URJoint *> Joints;

  UPROPERTY(EditAnywhere)
  TMap<FString, URLink *> Links;

  URLink *BaseLink;

  UPROPERTY(VisibleAnywhere)
  TMap<FString, URPluginComponent *> Plugins;

  TArray<URGraspComponent *> Grippers;
};
