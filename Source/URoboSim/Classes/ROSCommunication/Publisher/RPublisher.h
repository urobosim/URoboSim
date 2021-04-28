
#pragma once

#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
#include "Physics/RModel.h"
#include "Physics/RJoint.h"
#include "Physics/RLink.h"
#include "ROSCommunication/Service/Client/RServiceClient.h"
#include "Controller/ControllerType/JointController/RJointController.h"
#include "Controller/ControllerType/JointController/RGripperController.h"
#include "Controller/ControllerType/JointController/RHeadController.h"
#include "Controller/RController.h"
#include "Conversions.h"
#include "RPublisherParameter.h"
#include "RPublisher.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URPublisher : public UObject
{
    GENERATED_BODY()
public:


    URPublisher();
    virtual void DeInit();
    virtual void Publish(){};

    // static URPublisher* Init(FString Type, FString RosTopic, UObject* Owner, TSharedPtr<FROSBridgeHandler> InHandler);

    virtual void Init(UObject* InOwner, TSharedPtr<FROSBridgeHandler> InHandler, FString InRosTopic);
    virtual void Init(FString InHostIp, uint32 InPort, UObject* InOwner);

    virtual void SetPublishParameters(URPublisherParameter *&PublisherParameters);

    TSharedPtr<FROSBridgePublisher> Publisher;
	int Seq;

    UPROPERTY()
    FString ControllerName;

	UPROPERTY(EditAnywhere)
    FString Topic;
protected:



	UPROPERTY()
    FString MessageType;

        TSharedPtr<FROSBridgeHandler> Handler;

	virtual void SetMessageType(){};
	virtual void SetOwner(UObject* InOwner){};
	virtual void CreatePublisher();
};
