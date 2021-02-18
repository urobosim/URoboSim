#pragma once

#include "ROSCommunication/Publisher/RPublisher.h"
#include "Physics/RModel.h"
#include "ROSCommunication/RROSClient.h"
#include "EnvironmentStatePublisher.generated.h"

UCLASS()
class UROBOSIM_API UREnvironmentStatePublisher : public URPublisher
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UREnvironmentStatePublisher();

    virtual void Publish();
    TArray<FString> ListJointName;
protected:
    virtual void SetMessageType();
    virtual void SetOwner(UObject* InOwner);
    virtual void CreatePublisher();


private:
    TMap<FString, double> ListJointPosition;
    // double ListJointPosition;
};
