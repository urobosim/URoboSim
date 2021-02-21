#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "Physics/RModel.h"

#include "ROdomPublisher.generated.h"


UCLASS()
class UROBOSIM_API UROdomPublisher : public URPublisher
{
  GENERATED_BODY()
public:

  UROdomPublisher();

  virtual void Publish();

  UPROPERTY(EditAnywhere)
    FTransform FrameTransform;

  UPROPERTY(EditAnywhere)
    FString MapFrameId = "map";

  UPROPERTY(EditAnywhere)
    FString OdomFrameId = "odom_combined";

  UPROPERTY(EditAnywhere)
    FString BaseFrameId = "base_footprint";

  UPROPERTY(EditAnywhere)
    bool bProjectToGround = true;
 protected:

  virtual void SetMessageType();
  virtual void SetOwner(UObject* InOwner);

  UPROPERTY()
    ARModel* Owner;
};
