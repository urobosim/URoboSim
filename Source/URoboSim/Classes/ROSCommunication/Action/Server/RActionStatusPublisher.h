#pragma once

#include "RActionServer.h"
#include "RActionServerParameter.h"
// clang-format off
#include "RActionStatusPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URActionStatusPublisher : public URActionPublisher
{
  GENERATED_BODY()

public:
  URActionStatusPublisher();

public:
  void Publish() override;

  void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;
};
