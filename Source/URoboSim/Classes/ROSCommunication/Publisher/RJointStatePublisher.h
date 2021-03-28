#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Service/Client/RGetParamClient.h"
// clang-format off
#include "RJointStatePublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointStatePublisher : public URPublisher
{
  GENERATED_BODY()

public:
  URJointStatePublisher();

public:
  virtual void Publish();

protected:
  virtual void Init() override;

protected:
  UPROPERTY(EditAnywhere)
  FString FrameId;

private:
  UPROPERTY()
  TMap<FString, FJointState> JointStates;
};