#pragma once


#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvClient.h"
#include "PR2UnrealSimulator/RegisterRobot.h"
#include "PR2UnrealSimulator/UpdateWorld.h"
#include "ROSCommunication/RROSClientImpl.h"
#include "RUtilityClasses.h"
#include "Physics/RModel.h"
#include "RROSClient.generated.h"


UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URROSClient : public UObject
{
GENERATED_BODY()
public:
	virtual void CallService(){};

	virtual void Init(UObject* InModel, TSharedPtr<FROSBridgeHandler> InHandler, FString InName);
	virtual void Init(UObject* InModel, TArray<FString>* OutArray, TSharedPtr<FROSBridgeHandler> InHandler);

	virtual void SetParameters(float InSimTime, FJointState InParameters, FTransform InRobotPose){};
	virtual void Init(UObject* InModel){};
	virtual void Init(UObject* InModel, TArray<FString>* OutArray){};
protected:

	// UPROPERTY(EditAnywhere, Category = "ROS Service Server")
	// FString Name;


	TSharedPtr<FROSBridgeHandler> ROSHandler;

};

UCLASS()
class UROBOSIM_API URJointStateConfigurationClient : public URROSClient
{
GENERATED_BODY()

public:
	virtual void CallService();

	UPROPERTY()
	TArray<FString> JointNames;

	UPROPERTY(EditAnywhere)
	FString JointParamTopic;

	// UPROPERTY(EditAnywhere)
	// FString ConfigurationTargetName;

	virtual void Init(UObject* InModel, TArray<FString>* OutArray) override;
private:
	// UPROPERTY()
	// URControllerComponent* ConfigTarget;

	TSharedPtr<FROSJointStateConfigurationClient> ServiceClient;
	TSharedPtr<rosapi::GetParam::Request> Request;
	TSharedPtr<rosapi::GetParam::Response> Response;
};

