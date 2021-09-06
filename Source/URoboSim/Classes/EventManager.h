
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KnowRobInterface.h"
#include "EventManager.generated.h"


UCLASS()
class UROBOSIM_API UEventManagerComponent : public UActorComponent
{
  GENERATED_BODY()

public:

  UEventManagerComponent(){};


  // UPROPERTY(EditAnywhere, Instanced)
  // TArray<UEventManager*> EventMangers;

  // Called every frame
  virtual void BeginPlay() override;
protected:

};

// UCLASS(Blueprintable, DefaultToInstanced, hidecategories = Object, editinlinenew)
// class UROBOSIM_API UEventManager: public UObject
// {
//   GENERATED_BODY()

// public:
//   virtual void BeginPlay() override;
//   virtual void RegisterEvent(){};
//   virtual void Init(UKnowrobInterface* InKnowrobInterface){ KnowrobInterface = InKnowrobInterface};

//   UPROPERTY()
//   UKnowrobInterface* KnowrobInterface;
// };

// UCLASS(Blueprintable, DefaultToInstanced, hidecategories = Object, editinlinenew)
// class UROBOSIM_API UCollisionEventManager: public UEventManager
// {
//   GENERATED_BODY()

// public:
//   virtual void RegisterEvent();
//   virtual void Callback(FActorHitSignature InActorHitSignature);
// };
