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
  void Publish();

public:
  const URJointStatePublisherParameter *GetJointStatePublisherParameters() const { return Cast<URJointStatePublisherParameter>(PublisherParameters); }

protected:
  void Init() override;

private:
  UPROPERTY()
  TMap<FString, FJointState> JointStates;
};