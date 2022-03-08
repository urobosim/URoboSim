#pragma once

#include "RPublisher.h"
// clang-format off
#include "RTFPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URTFPublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  URTFPublisherParameter()
  {
    Topic = TEXT("/tf");
    MessageType = TEXT("tf2_msgs/TFMessage");
    MapFrameId = TEXT("map");
  }

public:
  UPROPERTY(EditAnywhere)
  FString MapFrameId;
};

UCLASS()
class UROBOSIM_API URTFPublisher : public URPublisher
{
  GENERATED_BODY()

public:
  URTFPublisher();

public:
  virtual void Publish();

  virtual void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;
  
public:
  UPROPERTY(EditAnywhere)
  FString MapFrameId;

protected:
  virtual void AddObject(AActor* InObject);

  virtual void SetObjects(TArray<AActor*> InObjects);

private:
  TArray<AActor*> Objects;
};
