#pragma once

#include "ROSCommunication/Action/Server/RActionStatusPublisher.h"
// clang-format off
#include "FJTAStatusPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URFJTAStatusPublisher : public URActionStatusPublisher
{
  GENERATED_BODY()

protected:
  virtual void Init() override;
};