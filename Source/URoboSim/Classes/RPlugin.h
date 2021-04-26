#pragma once

#include "Components/ActorComponent.h"
#include "Physics/RModel.h"
// clang-format off
#include "RPlugin.generated.h"
// clang-format on

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URPluginComponent : public UActorComponent
{
  GENERATED_BODY()
public:
  virtual void OnComponentCreated() override;

  ARModel* GetOwner();

protected:
  virtual void RegisterPlugin();
};
