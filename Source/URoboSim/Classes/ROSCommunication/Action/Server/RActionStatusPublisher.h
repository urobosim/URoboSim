#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "RActionServerParameter.h"
#include "Controller/RController.h"
// clang-format off
#include "RActionStatusPublisher.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URActionStatusPublisher : public URPublisher
{
  GENERATED_BODY()

public:
  void Publish() override;

public:
  const URActionStatusPublisherParameter *GetActionStatusPublisherParameters() const { return Cast<URActionStatusPublisherParameter>(PublisherParameters); }

protected:
  URController *Controller;
};
