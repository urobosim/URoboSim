#pragma once

#include "RPublisher.h"
// clang-format off
#include "ROdomPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API UROdomPublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  UROdomPublisherParameter()
  {
    Topic = TEXT("/tf");
    MessageType = TEXT("tf2_msgs/TFMessage");
    MapFrameId = TEXT("map");
    OdomFrameId = TEXT("odom_combined");
    BaseFrameId = TEXT("base_footprint");
    bProjectToGround = true;
  }

public:
  UPROPERTY(EditAnywhere)
  FTransform FrameTransform;

  UPROPERTY(EditAnywhere)
  FString MapFrameId;

  UPROPERTY(EditAnywhere)
  FString OdomFrameId;

  UPROPERTY(EditAnywhere)
  FString BaseFrameId;

  UPROPERTY(EditAnywhere)
  bool bProjectToGround;
};

UCLASS()
class UROBOSIM_API UROdomPublisher : public URPublisher
{
  GENERATED_BODY()

public:
  UROdomPublisher();

public:
  virtual void Publish() override;

  virtual void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

public:
  UPROPERTY(EditAnywhere)
  FTransform FrameTransform;

  UPROPERTY(EditAnywhere)
  FString MapFrameId;

  UPROPERTY(EditAnywhere)
  FString OdomFrameId;

  UPROPERTY(EditAnywhere)
  FString BaseFrameId;

  UPROPERTY(EditAnywhere)
  bool bProjectToGround;
};
