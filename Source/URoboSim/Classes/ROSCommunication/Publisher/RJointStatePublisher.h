#pragma once

#include "RPublisher.h"
#include "RJointStatePublisherParameter.h"
// clang-format off
#include "RJointStatePublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URJointStatePublisher final : public URPublisher
{
  GENERATED_BODY()

public:
  URJointStatePublisher();

public:
  void Publish() override;

protected:
  void Init() override;

public:
  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

public:
  UPROPERTY()
  TMap<FString, FJointState> JointStates;

private:
  UPROPERTY(EditAnywhere)
  FString FrameId; 
};