#pragma once

#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
#include "ROSBridgeSrvServer.h"
#include "PR2UnrealSimulator/RegisterRobot.h"
#include "ROSCommunication/RROSServiceServer.h"
#include "Controller/RControllerComponent.h"
#include "RROSService.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URROSService : public UObject
{
GENERATED_BODY()
public:
	// virtual void CallService(){};
	virtual void Init(UObject* InModel, TSharedPtr<FROSBridgeHandler> InHandler, FString InName);

	TSharedPtr<FROSBridgeSrvServer> ServiceServer;
protected:

	UPROPERTY(EditAnywhere, Category = "ROS Service Server")
	FString Name;

	UPROPERTY()
	FString Type;

	TSharedPtr<FROSBridgeHandler> ROSHandler;

	virtual void SetOwner(UObject* Owner){};
	virtual void SetType(){};
	virtual void CreateServiceServer(){};
};

UCLASS()
class UROBOSIM_API URROSSimulationCommandsService : public URROSService
{
GENERATED_BODY()
public:


protected:
	UPROPERTY()
	URControllerComponent* Owner;

	virtual void SetOwner(UObject* InOwner);
	virtual void SetType();
	virtual void CreateServiceServer();


};

// UCLASS()
// class UROBOSIM_API URROSRobotRegistrationService : public URROSService
// {
// GENERATED_BODY()
//
// public:
//
// protected:
// 	// virtual void Init() override;
// private:
// 	TSharedPtr<FROSRobotRegistrationServer> ServiceServer;
// };
//
// UCLASS()
// class UROBOSIM_API URROSServiceFactory: public UObject
// {
//     GENERATED_BODY()
// public:
// 	static URROSService* CreateInstance(FString Type, UObject* Owner);
//
// };
