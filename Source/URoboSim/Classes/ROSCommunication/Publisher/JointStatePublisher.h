#pragma once

#include "RPublisher.h"
#include "Physics/RModel.h"
#include "ROSCommunication/Service/Client/RROSClient.h"
#include "JointStatePublisher.generated.h"

UCLASS()
class UROBOSIM_API URJointStatePublisherParameter : public URPublisherParameter
{
  GENERATED_BODY()

public:
  URJointStatePublisherParameter()
  {
    Topic = TEXT("/joint_states");
    JointParamTopic = TEXT("/hardware_interface/joints");
  }

  UPROPERTY(EditAnywhere)
  FString JointParamTopic;
};

UCLASS()
class UROBOSIM_API URJointStatePublisher : public URPublisher
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    URJointStatePublisher();

    virtual void SetPublishParameters(URPublisherParameter *&PublisherParameters) override;

    virtual void Publish();
    TArray<FString> ListJointName;
protected:
    virtual void SetMessageType();
    virtual void SetOwner(UObject* InOwner);
    virtual void CreatePublisher();

    UPROPERTY()
      URJointStateConfigurationClient* ConfigClient;

    UPROPERTY(EditAnywhere)
      FString JointParamTopic;

    UPROPERTY()
      ARModel* Owner;
private:
    TArray<double> ListJointPosition, ListJointVelocity, ListJointEffort;
};
