
#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "Controller/RController.h"
#include "ROSCommunication/Publisher/RTFPublisher.h"
#include "POAResultPublisher.generated.h"

UCLASS()
class UROBOSIM_API URPerceiveObjectActionResultPublisher : public URPublisher
{
  GENERATED_BODY()

public:
    virtual void Publish();

protected:
    virtual void SetMessageType();
    virtual void SetOwner(UObject* InOwner);

    UPROPERTY()
      URCameraController* Owner;

    UPROPERTY()
      URTFPublisher* TFPublisher;
};
