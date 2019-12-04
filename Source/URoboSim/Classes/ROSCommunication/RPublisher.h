
#pragma once

#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
#include "Physics/RModel.h"
#include "Physics/RJoint.h"
#include "Physics/RLink.h"
#include "ROSCommunication/RROSClient.h"
#include "Controller/RJointController.h"
#include "Controller/RGripperController.h"
#include "Controller/RHeadController.h"
#include "Controller/RController.h"
#include "Sensor/R2DLidar.h"
#include "Conversions.h"
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

    TSharedPtr<FROSBridgePublisher> Publisher;
	int Seq;

    UPROPERTY()
    FString ControllerName;
protected:


	UPROPERTY(EditAnywhere)
    FString Topic;

	UPROPERTY()
    FString MessageType;

	TSharedPtr<FROSBridgeHandler> Handler;

	virtual void SetMessageType(){};
	virtual void SetOwner(UObject* InOwner){};
	virtual void CreatePublisher();
};

UCLASS()
class UROBOSIM_API URActionStatePublisher : public URPublisher
{
GENERATED_BODY()

public:

virtual void Publish();
protected:
virtual void SetMessageType();
virtual void SetOwner(UObject* InOwner){};

UPROPERTY()
URController* Owner;
};

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



UCLASS()
class UROBOSIM_API URPR2HeadActionStatePublisher : public URActionStatePublisher
{
GENERATED_BODY()

protected:
virtual void SetOwner(UObject* InOwner);

};

UCLASS()
class UROBOSIM_API URPR2HeadActionFeedbackPublisher : public URPublisher
{
    GENERATED_BODY()

public:

    virtual void Publish();
protected:
	virtual void SetMessageType();
	virtual void SetOwner(UObject* InOwner);

	UPROPERTY()
	URHeadTrajectoryController* Owner;
};

UCLASS()
class UROBOSIM_API URPR2HeadActionResultPublisher : public URPublisher
{
    GENERATED_BODY()

public:

    virtual void Publish();
protected:
	virtual void SetMessageType();
	virtual void SetOwner(UObject* InOwner);

	UPROPERTY()
	URHeadTrajectoryController* Owner;
};

UCLASS()
class UROBOSIM_API URPR2GripperCommandActionStatePublisher : public URActionStatePublisher
{
GENERATED_BODY()

protected:
virtual void SetOwner(UObject* InOwner);

};

UCLASS()
class UROBOSIM_API URPR2GripperCommandActionFeedbackPublisher : public URPublisher
{
    GENERATED_BODY()

public:

    virtual void Publish();
protected:
	virtual void SetMessageType();
	virtual void SetOwner(UObject* InOwner);

	UPROPERTY()
	URGripperController* Owner;
};

UCLASS()
class UROBOSIM_API URPR2GripperCommandActionResultPublisher : public URPublisher
{
    GENERATED_BODY()

public:

    virtual void Publish();
protected:
	virtual void SetMessageType();
	virtual void SetOwner(UObject* InOwner);

	UPROPERTY()
	URGripperController* Owner;
};

UCLASS()
class UROBOSIM_API URGripperCommandActionStatePublisher : public URActionStatePublisher
{
GENERATED_BODY()

protected:
virtual void SetOwner(UObject* InOwner){};

};

UCLASS()
class UROBOSIM_API URGripperCommandActionFeedbackPublisher : public URPublisher
{
    GENERATED_BODY()

public:

    virtual void Publish(){};
protected:
	virtual void SetMessageType(){};
	virtual void SetOwner(UObject* InOwner){};

	UPROPERTY()
	URGripperController* Owner;
};

UCLASS()
class UROBOSIM_API URGripperCommandActionResultPublisher : public URPublisher
{
    GENERATED_BODY()

public:

    virtual void Publish(){};
protected:
	virtual void SetMessageType(){};
	virtual void SetOwner(UObject* InOwner){};

	UPROPERTY()
	URGripperController* Owner;
};

UCLASS()
class UROBOSIM_API URPerceiveObjectActionFeedbackPublisher : public URPublisher
{
    GENERATED_BODY()

public:

    virtual void Publish();
protected:
	virtual void SetMessageType();
	virtual void SetOwner(UObject* InOwner);

	UPROPERTY()
	URCameraController* Owner;
};

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
};

UCLASS()
class UROBOSIM_API URPerceiveObjectActionStatePublisher : public URActionStatePublisher
{
    GENERATED_BODY()

protected:
	virtual void SetOwner(UObject* InOwner);

	// UPROPERTY()
	// URCameraController* Owner;
};

UCLASS()
class UROBOSIM_API UROdomPublisher : public URPublisher
{
    GENERATED_BODY()
public:
    virtual void Publish();
    UROdomPublisher();

	UPROPERTY(EditAnywhere)
	FTransform FrameTransform;

	UPROPERTY(EditAnywhere)
	FString MapFrameId = "map";

	UPROPERTY(EditAnywhere)
	FString OdomFrameId = "odom_combined";

	UPROPERTY(EditAnywhere)
	FString BaseFrameId = "base_footprint";
protected:
	virtual void SetMessageType();
	virtual void SetOwner(UObject* InOwner);

	UPROPERTY()
	ARModel* Owner;
};


UCLASS()
class UROBOSIM_API URJointTrajectoryFeedbackPublisher : public URPublisher
{
    GENERATED_BODY()

public:
    virtual void Publish();
    URJointTrajectoryFeedbackPublisher();
protected:
	virtual void SetMessageType();
	virtual void SetOwner(UObject* InOwner);

	UPROPERTY()
	URJointController* Owner;

	// UPROPERTY()
	// URROSClient* ConfigClient;
	UPROPERTY()
	URJointStateConfigurationClient* ConfigClient;

	UPROPERTY(EditAnywhere)
	FString JointParamTopic;
private:

    TArray<FString> ListJointName;
    TArray<double> ListJointPosition, ListJointVelocity;

};

UCLASS()
class UROBOSIM_API URFollowJointTrajectoryActionStatePublisher : public URActionStatePublisher
{
GENERATED_BODY()

protected:
virtual void SetOwner(UObject* InOwner);

};



UCLASS()
class UROBOSIM_API URFollowTrajectoryActionResultPublisher : public URPublisher
{
    GENERATED_BODY()

public:


    virtual void Publish();
protected:
	virtual void SetMessageType();
	virtual void SetOwner(UObject* InOwner);

	UPROPERTY()
	URJointController* Owner;

};

UCLASS()
class UROBOSIM_API URJointTrajectoryControllerStatePublisher : public URJointTrajectoryFeedbackPublisher
{
    GENERATED_BODY()
public:
      URJointTrajectoryControllerStatePublisher();
    virtual void Publish();
protected:
    virtual void SetMessageType();
};

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
