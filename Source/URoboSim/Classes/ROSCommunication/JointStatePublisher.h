#pragma once

#include "ROSCommunication/RPublisher.h"
#include "Physics/RModel.h"
#include "ROSCommunication/RROSClient.h"
#include "JointStatePublisher.generated.h"

UCLASS()
class UROBOSIM_API URJointStatePublisher : public URPublisher
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    URJointStatePublisher();

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
