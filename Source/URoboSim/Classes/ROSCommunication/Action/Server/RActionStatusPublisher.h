#pragma once

#include "Controller/RControllerComponent.h"
#include "RActionServerParameter.h"
#include "ROSCommunication/Publisher/RPublisher.h"
// clang-format off
#include "RActionStatusPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URActionStatusPublisher : public URPublisher
{
  GENERATED_BODY()

public:
  URActionStatusPublisher();

public:
  void Publish() override;

  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

protected:
  virtual void Init() override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;

  UPROPERTY(EditAnywhere)
  FString ControllerName;

protected:
  URController *Controller;
};
