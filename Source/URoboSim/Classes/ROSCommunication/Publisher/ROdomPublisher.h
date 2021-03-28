#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
// clang-format off
#include "ROdomPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API UROdomPublisher : public URPublisher
{
  GENERATED_BODY()

public:
  UROdomPublisher();

public:
  virtual void Publish() override;

protected:
  virtual void Init() override;

private:
	virtual void CalculateOdomStates();

protected:
  UPROPERTY(EditAnywhere)
  FString FrameId;

  UPROPERTY(EditAnywhere)
	TArray<FString> FrameNames;

  UPROPERTY(EditAnywhere)
  bool bProjectToGround = true;

  UPROPERTY()
	TArray<double> OdomPosition;

	UPROPERTY()
	TArray<double> OdomVelocity;
};
