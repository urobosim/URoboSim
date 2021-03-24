#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/Service/Client/RJointStateConfigurationClient.h"
// clang-format off
#include "RJointStatePublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointStatePublisher : public URPublisher
{
  GENERATED_BODY()

public:
  // Sets default values for this component's properties
  URJointStatePublisher();

  virtual void Publish();

protected:
  virtual void SetMessageType();

  void SetOwner(UObject *&InOwner) override;

protected:
  UPROPERTY(EditAnywhere)
  FString FrameId;

private:
  UPROPERTY()
  TMap<FString, FJointState> JointStates;
};