#include "ROSCommunication/Service/Server/RServiceServer.h"

void URServiceServer::SetServiceServerParameters(URServiceServerParameter *&ServiceServerParameters)
{
	if (ServiceServerParameters)
  {
    Name = ServiceServerParameters->Name;
    Type = ServiceServerParameters->Type;
  }
}

void URServiceServer::Init()
{
  SetOwner(GetOuter());
	CreateServiceServer();
	if (Handler.IsValid())
	{
		Handler->AddServiceServer(ServiceServer);
	}
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

// URServiceServer* URServiceServerFactory::CreateInstance(FString InType, UObject* InOwner)
// {
// 		URServiceServer* Service = nullptr;
//
// 			return Service;
// }
//
