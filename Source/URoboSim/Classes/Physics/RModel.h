// Copyright 2018, Institute for Artificial Intelligence - University of Bremen
// Author: Michael Neumann

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RUtilityClasses.h"
#include "RGraspComponent.h"
#include "Physics/RLink.h"
#include "RModel.generated.h"


class USDFModel;
class USDFJoint;
class USDFLink;
class URJoint;
// class URLink;

USTRUCT()
struct FModelInformation
{
  GENERATED_BODY()
  public:

};

UCLASS()
class UROBOSIM_API ARModel : public AActor
{
  GENERATED_BODY()

    public:
    // Sets default values for this actor's properties
    ARModel();

  // Destructor
  ~ARModel();

  UPROPERTY(EditAnywhere)
    TMap<FString, URJoint*> Joints;

  UPROPERTY(EditAnywhere)
    TMap<FString, URLink*> Links;

  UPROPERTY()
  URLink* BaseLink;

  UPROPERTY(VisibleAnywhere)
    TMap<FString, UActorComponent*> Plugins;

  virtual FJointState GetJointState();

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;


  UPROPERTY()
    TArray<URGraspComponent*> Grippers;
 public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  void AddJoint(URJoint* Joint);
  void AddLink(URLink* Link);
  // Load model

};
