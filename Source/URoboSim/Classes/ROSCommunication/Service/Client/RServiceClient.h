#pragma once

#include "ROSCommunication/RROSCommunication.h"
#include "RobotInterface.h"
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
class UROBOSIM_API UServiceClient : public UROSCommunication
{
  GENERATED_BODY()

public:
  void Tick() override;

  virtual void SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters);

  virtual void CallService(){}

protected:

  virtual void CreateServiceClient(){}

  virtual void Init() override;

  UPROPERTY(EditAnywhere)
  FString ServiceName;

  UPROPERTY(EditAnywhere)
  FString ServiceType;
};

UCLASS()
class UROBOSIM_API URServiceClient : public UServiceClient, public IRobotInterface
{
  GENERATED_BODY()

protected:
  virtual void Init() override;

};
