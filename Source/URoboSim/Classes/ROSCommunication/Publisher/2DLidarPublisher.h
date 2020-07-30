
#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "Sensor/R2DLidar.h"
#include "2DLidarPublisher.generated.h"

UCLASS()
class UROBOSIM_API UR2DLidarPublisher : public URPublisher
{
  GENERATED_BODY()

public:

    UR2DLidarPublisher();

    virtual void Publish();

    UPROPERTY(EditAnywhere)
      FString SensorName;
protected:
    virtual void SetMessageType();
    virtual void SetOwner(UObject* InOwner);

    UPROPERTY()
      UR2DLidar* Owner;
};
