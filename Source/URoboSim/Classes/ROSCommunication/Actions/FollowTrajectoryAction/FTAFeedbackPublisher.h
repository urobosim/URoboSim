#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "ROSCommunication/RROSClient.h"
#include "Controller/RJointController.h"
#include "FTAFeedbackPublisher.generated.h"

UCLASS()
class UROBOSIM_API URJointTrajectoryFeedbackPublisher : public URPublisher
{
  GENERATED_BODY()

public:
  URJointTrajectoryFeedbackPublisher();
  virtual void Publish();

protected:
  virtual void SetMessageType();
  virtual void SetOwner(UObject* InOwner);

  UPROPERTY()
    URJointController* Owner;

  UPROPERTY()
    URJointStateConfigurationClient* ConfigClient;

  UPROPERTY(EditAnywhere)
    FString JointParamTopic;

 private:

  TArray<FString> ListJointName;
  TArray<double> ListJointPosition, ListJointVelocity;

};
