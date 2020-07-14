
#pragma once

#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
#include "Physics/RModel.h"
#include "Conversions.h"
#include "RDonbotOdomPublisher.generated.h"

UCLASS()
class UROBOSIM_API URDonbotOdomPublisher : public URPublisher
{
    GENERATED_BODY()
public:
    virtual void Publish();
    URDonbotOdomPublisher();

	UPROPERTY(EditAnywhere)
	FTransform FrameTransform;

	UPROPERTY(EditAnywhere)
	FString MapFrameId = "map";

	UPROPERTY(EditAnywhere)
	FString OdomFrameXId = "odom_x";

	UPROPERTY(EditAnywhere)
	FString OdomFrameYId = "odom_y";

	UPROPERTY(EditAnywhere)
	FString OdomFrameZId = "odom_z";

	UPROPERTY(EditAnywhere)
	FString BaseFrameId = "base_footprint";

        UPROPERTY(EditAnywhere)
          bool bProjectToGround = true;
protected:
	virtual void SetMessageType();
	virtual void SetOwner(UObject* InOwner);

        UPROPERTY()
        TArray<FString> FrameNames;

        UPROPERTY()
        TArray<double> JointStates;

	UPROPERTY()
	ARModel* Owner;
};
