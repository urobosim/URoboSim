#pragma once

#include "Components/ActorComponent.h"
#include "Physics/RModel.h"
// clang-format off
#include "RPluginComponent.generated.h"
// clang-format on

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UROBOSIM_API URPluginComponent : public UActorComponent
{
  GENERATED_BODY()
public:
  virtual void OnComponentCreated() override;

  virtual void BeginPlay() override;

  ARModel *GetOwner() const;

protected:
  virtual void RegisterPlugin();

  virtual void Init() {}
};
