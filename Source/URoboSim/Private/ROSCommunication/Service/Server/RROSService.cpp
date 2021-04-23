#include "ROSCommunication/RROSService.h"

void URROSService::Init(UObject* InOwner, TSharedPtr<FROSBridgeHandler> InHandler, FString InName)
{
	ROSHandler = InHandler;
	SetOwner(InOwner);
	SetType();
	CreateServiceServer();
	ROSHandler->AddServiceServer(ServiceServer);
}

void URROSService::SetROSServiceParameters(URROSServiceParameter *&ROSServiceParameters)
{
	if (ROSServiceParameters)
  {
    Name = ROSServiceParameters->Name;
    Type = ROSServiceParameters->Type;
  }
}

void URROSSimulationCommandsService::SetOwner(UObject* InOwner)
{
	Owner = Cast<URControllerComponent>(InOwner);
	if(!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner of SimulationCommands no ControllerComponent"));
	}
}

void URROSSimulationCommandsService::CreateServiceServer()
{
	ServiceServer =	MakeShareable<FROSSimulationCommandsServer>(new FROSSimulationCommandsServer(Name, Type, Owner));
}

void URROSSimulationCommandsService::SetType()
{
	Type = TEXT("u_robo_sim_communication/SimulationCommands");
}

// URROSService* URROSService::Init(FString InType, UObject* InOwner, TSharedPtr<FROSBridgeHandler> InHandler)
// {
// 		URROSService* Service = URROSServiceFactory::CreateInstance(InType, InOwner);
// 			if(Service)
// 					{
// 								Service->ROSHandler = InHandler;
// 									}
// 				return Service;
// }


// void URROSRobotRegistrationService::Init()
// {
// 		ServiceServer =	MakeShareable<FROSRobotRegistrationServer>(new FROSRobotRegistrationServer(Name, TEXT("unreal_controller_manager/RegisterRobot")));
//
// 			ROSHandler->AddServiceServer(ServiceServer);
//
// }

// URROSService* URROSServiceFactory::CreateInstance(FString InType, UObject* InOwner)
// {
// 		URROSService* Service = nullptr;
//
// 			return Service;
// }
//
