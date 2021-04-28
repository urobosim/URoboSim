#include "ROSCommunication/Service/Server/RServiceServer.h"

void URServiceServer::Init(UObject* InOwner, TSharedPtr<FROSBridgeHandler> InHandler, FString InName)
{
	ROSHandler = InHandler;
	SetOwner(InOwner);
	SetType();
	CreateServiceServer();
	ROSHandler->AddServiceServer(ServiceServer);
}

void URServiceServer::SetServiceServerParameters(URServiceServerParameter *&ServiceServerParameters)
{
	if (ServiceServerParameters)
  {
    Name = ServiceServerParameters->Name;
    Type = ServiceServerParameters->Type;
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

// URServiceServer* URServiceServer::Init(FString InType, UObject* InOwner, TSharedPtr<FROSBridgeHandler> InHandler)
// {
// 		URServiceServer* Service = URServiceServerFactory::CreateInstance(InType, InOwner);
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

// URServiceServer* URServiceServerFactory::CreateInstance(FString InType, UObject* InOwner)
// {
// 		URServiceServer* Service = nullptr;
//
// 			return Service;
// }
//
