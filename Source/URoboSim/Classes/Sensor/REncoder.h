#pragma once

#include "REncoder.generated.h"

UCLASS(ClassGroup=(RobotSensor), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API UREncoder : public UObject
{

  GENERATED_BODY()
 public:
    UREncoder();

  virtual void Test();
  /*Resets Value to zero*/
  virtual void ResetValue();

  /*Updates the AccumulatedValue
    */
  void UpdateValue(float NewValue);

  /*Returns the value of the encoder
    if bUseModulo = true return the value
    in [+- pi]*/
  virtual float GetValue(bool bNormalize=false);

 protected:

  virtual void UpdateValue();
  virtual float GetAccumulatedValue();

  UPROPERTY()
    float OldValue;

  UPROPERTY()
    float NewValue;

  UPROPERTY()
    float AccumulatedValue;
};

UCLASS(ClassGroup=(RobotSensor), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API URLinearEncoder : public UREncoder
{
  GENERATED_BODY()

};

UCLASS(ClassGroup=(RobotSensor), meta=(BlueprintSpawnableComponent))
class UROBOSIM_API URAngularEncoder : public UREncoder
{
  GENERATED_BODY()

public:



protected:

  virtual void UpdateValue() override;

};
