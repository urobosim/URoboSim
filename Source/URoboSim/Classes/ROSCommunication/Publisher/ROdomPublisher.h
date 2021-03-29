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
  UROdomPublisher();

public:
  void Publish() override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

protected:
  void Init() override;

private:
	void CalculateOdomStates();

protected:
  UPROPERTY(EditAnywhere)
	TArray<FString> FrameNames;

  UPROPERTY(EditAnywhere)
  bool bProjectToGround = true;

  UPROPERTY()
	TArray<double> OdomPosition;

	UPROPERTY()
	TArray<double> OdomVelocity;
};
