#pragma once

#include "Physics/RModel.h"
// clang-format off
#include "RSensor.generated.h"
// clang-format on

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URSensorParameter : public UObject
{
  GENERATED_BODY()

};

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URSensor : public UObject
{
  GENERATED_BODY()

public:
  // Called every frame
  virtual void Tick(const float &InDeltaTime){};

  virtual void Init();

public:
  ARModel *GetOwner();

  virtual void SetSensorParameters(URSensorParameter *&SensorParameters) {}

public:
  bool bPublishResult = false;

private:
  ARModel *Owner;
};