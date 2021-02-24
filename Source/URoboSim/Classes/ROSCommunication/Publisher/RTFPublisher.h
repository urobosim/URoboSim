#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "RTFPublisher.generated.h"


UCLASS()
class UROBOSIM_API URTFPublisher : public URPublisher
{
  GENERATED_BODY()
public:

  URTFPublisher();

  virtual void Publish();
  virtual void AddObject(AActor* InObject);
  virtual void SetObjects(TArray<AActor*> InObject);

  UPROPERTY(EditAnywhere)
    FString MapFrameId = "map";

 protected:

  virtual void SetMessageType();
  virtual void SetOwner(UObject* InOwner);

  UPROPERTY()
  TArray<AActor*> ObjectList;
};
