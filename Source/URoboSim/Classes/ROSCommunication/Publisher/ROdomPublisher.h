#pragma once

#include "RPublisher.h"
#include "ROdomPublisherParameter.h"
// clang-format off
#include "ROdomPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API UROdomPublisher final : public URPublisher
{
  GENERATED_BODY()

public:
  UROdomPublisher();

public:
  void Publish() override;

protected:
  void Init() override;

public:
  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

  UPROPERTY(EditAnywhere)
	TArray<FString> FrameNames;

private:
	void CalculateOdomStates();

private:
  UPROPERTY(EditAnywhere)
  bool bProjectToGround = true;

	TArray<double> OdomPosition;

	TArray<double> OdomVelocity;
};
