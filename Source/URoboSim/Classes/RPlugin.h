#pragma once

#include "Components/ActorComponent.h"
#include "Physics/RModel.h"
#include "RPlugin.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API URPluginComponent : public UActorComponent
{
    GENERATED_BODY()
public:

    virtual void OnComponentCreated() override;

protected:

    virtual void RegisterPlugin();
    virtual FString GetPluginName();

    UPROPERTY()
      ARModel* Model;

};
