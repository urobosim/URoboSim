#pragma once

#include "ROSCommunication/RROSCommunication.h"
#include "ROSBridgeSrvClient.h"
// clang-format off
#include "RServiceClient.generated.h"
// clang-format on

UCLASS(BlueprintType, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URServiceClientParameter : public UObject
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere)
  FString ServiceName;

  UPROPERTY(EditAnywhere)
  FString ServiceType;
};

UCLASS()
class UROBOSIM_API URServiceClient : public URROSCommunication
{
  GENERATED_BODY()

public:
  void Tick() override;

public:
  virtual void SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters);

  virtual void CallService(){}

protected:
  UPROPERTY(EditAnywhere)
  FString ServiceName;

  UPROPERTY(EditAnywhere)
  FString ServiceType;
};