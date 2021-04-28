#pragma once

#include "ROSBridgeHandler.h"
#include "ROSBridgePublisher.h"
#include "ROSBridgeSrvServer.h"
#include "PR2UnrealSimulator/RegisterRobot.h"
#include "RServiceServerParameter.h"
#include "ROSCommunication/Service/Server/RServiceServerImpl.h"
#include "Controller/RControllerComponent.h"
#include "RServiceServer.generated.h"

UCLASS(Blueprintable, DefaultToInstanced, collapsecategories, hidecategories = Object, editinlinenew)
class UROBOSIM_API URServiceServer : public UObject
{
GENERATED_BODY()
public:
	// virtual void CallService(){};
	virtual void Init(UObject* InModel, TSharedPtr<FROSBridgeHandler> InHandler, FString InName);

	virtual void SetServiceServerParameters(URServiceServerParameter *&ServiceServerParameters);

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
class UROBOSIM_API URROSSimulationCommandsService : public URServiceServer
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
// class UROBOSIM_API URROSRobotRegistrationService : public URServiceServer
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
// class UROBOSIM_API URServiceServerFactory: public UObject
// {
//     GENERATED_BODY()
// public:
// 	static URServiceServer* CreateInstance(FString Type, UObject* Owner);
//
// };
