#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
// clang-format off
#include "ROdomPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API UROdomPublisher final : public URPublisher
{
  GENERATED_BODY()

public:
  void Publish() override;

protected:
  void Init() override;

public:
  const UROdomPublisherParameter *GetOdomPublisherParameters() const { return Cast<UROdomPublisherParameter>(PublisherParameters); }

private:
	void CalculateOdomStates();

protected:
  UPROPERTY()
	TArray<double> OdomPosition;

	UPROPERTY()
	TArray<double> OdomVelocity;
};
