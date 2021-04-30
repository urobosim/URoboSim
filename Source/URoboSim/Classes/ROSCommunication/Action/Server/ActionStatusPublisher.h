#pragma once

#include "RActionServer.h"
// clang-format off
#include "ActionStatusPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URActionStatusPublisher : public URActionPublisher
{
  GENERATED_BODY()

public:
  URActionStatusPublisher();

public:
  void Publish() override;

public:
  UPROPERTY(EditAnywhere)
  FString FrameId;
};
