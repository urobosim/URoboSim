#pragma once


#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvClient.h"
#include "ROSCommunication/Service/Client/RROSClientImpl.h"
#include "RUtilityClasses.h"
#include "RROSClientParameter.h"
#include "Controller/RControllerComponent.h"
#include "Controller/ControllerType/JointController/RJointController.h"
#include "RROSClient.generated.h"


UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URROSClient : public UObject
{
GENERATED_BODY()
public:
	virtual void CallService(){};

	virtual void Init(UObject* InControllerComp, TSharedPtr<FROSBridgeHandler> InHandler);
	virtual void Init(UObject* InControllerComp, TArray<FString>* OutArray, TSharedPtr<FROSBridgeHandler> InHandler);

  virtual void SetROSClientParameters(URROSClientParameter *&ROSClientParameters){}

	virtual void SetParameters(float InSimTime, FJointState InParameters, FTransform InRobotPose){};
	virtual void Init(UObject* InControllerComp){};
	virtual void Init(UObject* InControllerComp, TArray<FString>* OutArray){};
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

// UPROPERTY()
// TArray<FString>* JointNames = nullptr;

   UPROPERTY(EditAnywhere)
     FString JointParamTopic;

   // virtual void Init(UObject* InControllerComp, TA) override;
 virtual void Init(UObject* InModel, TArray<FString>* OutArray) override;

 private:

   TSharedPtr<FROSJointStateConfigurationClient> ServiceClient;
   TSharedPtr<rosapi::GetParam::Request> Request;
   TSharedPtr<rosapi::GetParam::Response> Response;
};

UCLASS()
class UROBOSIM_API URJointControllerConfigurationClientParameter : public URROSClientParameter
{
  GENERATED_BODY()

public:
  URJointControllerConfigurationClientParameter()
  {
    JointParamTopic = TEXT("/whole_body_controller/joints");
    LimitParamTopic = TEXT("/robot_description");
  }

public:
  UPROPERTY(EditAnywhere)
  FString JointParamTopic;
  
  UPROPERTY(EditAnywhere)
  FString LimitParamTopic;
};

UCLASS()
class UROBOSIM_API URJointControllerConfigurationClient : public URROSClient
{
  GENERATED_BODY()

public:
    URJointControllerConfigurationClient();

  virtual void SetROSClientParameters(URROSClientParameter *&ROSClientParameters) override;

   UPROPERTY()
     URControllerComponent* ControllerComp;

   UPROPERTY(EditAnywhere)
     FString JointParamTopic;

   UPROPERTY(EditAnywhere)
     FString LimitParamTopic;

   virtual void CallService();

    virtual void Init(UObject* InControllerComp) override;
    virtual void CreateClient();

private:
    TSharedPtr<FROSJointControllerConfigurationClient> JointServiceClient;
    TSharedPtr<FROSJointLimitControllerConfigurationClient> JointLimitServiceClient;
    TSharedPtr<rosapi::GetParam::Request> JointRequest;
    TSharedPtr<rosapi::GetParam::Response> JointResponse;

    TSharedPtr<rosapi::GetParam::Request> LimitRequest;
    TSharedPtr<rosapi::GetParam::Response> LimitResponse;
};
