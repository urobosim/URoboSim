
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ROSCommunication/KnowRobInterface.h"
#include "KnowrobEventManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API UEventManagerComponent : public UActorComponent
{
  GENERATED_BODY()

public:

  UPROPERTY(EditAnywhere, Instanced)
  TArray<UEventManager*> EventManagers;

  // Called every frame
  virtual void BeginPlay() override;


  UPROPERTY(EditAnywhere)
    float RepitionDelay = 1.;

protected:


};

UCLASS(Blueprintable, DefaultToInstanced, hidecategories = Object, editinlinenew)
class UROBOSIM_API UEventManager: public UObject
{
  GENERATED_BODY()

public:
  virtual void RegisterEvent(){};
  virtual void Init(UKnowrobInterface* InKnowrobInterface, UObject* InObject);
  virtual void SetObject(UObject* InObject){};

  UPROPERTY()
  UKnowrobInterface* KnowrobInterface;
};

UCLASS(Blueprintable, DefaultToInstanced, hidecategories = Object, editinlinenew)
class UROBOSIM_API UCollisionEventManager: public UEventManager
{
  GENERATED_BODY()

public:
  virtual void RegisterEvent();
  virtual void SetObject(UObject* InObject) override;

  UFUNCTION()
  virtual void Callback(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

  UPROPERTY(EditAnywhere)
    float RepitionDelay = 1.;

  UPROPERTY(EditAnywhere)
    float LastCallback = 0.;

  UPROPERTY()
    AActor* Object;
};
