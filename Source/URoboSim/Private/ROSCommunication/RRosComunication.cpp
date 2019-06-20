#include "ROSCommunication/RRosComunication.h"
#include "Controller/RController.h"

FRROSComunicationContainer::FRROSComunicationContainer()
{
	WebsocketIPAddr = TEXT("127.0.0.1");
	WebsocketPort = 9090;
	RobotName = TEXT("pr2");
}


void FRROSComunicationContainer::Init()
{
	InitHandler();
    // // Owner = Cast<ARRobot>(GetOwner());
	if(Handler.IsValid())
	{
		Handler->Connect();
		InitAllPublisher();
		InitAllSubscriber();
		InitAllServiceProvider();
		InitAllClients();
		InitAllActionServer();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No FROSBridgeHandler created."));
	}
}


void FRROSComunicationContainer::InitAllServiceProvider()
{
    for(auto& ServiceProvider : ServiceProviderList)
    {
		ServiceProvider.Value->Init(ControllerComponent, Handler, ServiceProvider.Key);
    }
}

void FRROSComunicationContainer::InitAllPublisher()
{
    for(auto& Publisher : PublisherList)
    {
		Publisher.Value->Init(WebsocketIPAddr, WebsocketPort, ControllerComponent->GetOwner());
    }
}

void FRROSComunicationContainer::InitAllSubscriber()
{
	for(auto& Subscriber : SubscriberList)
	{
		Subscriber.Value->Init(ControllerComponent->GetOwner(), Handler);
	}
}

void FRROSComunicationContainer::InitAllClients()
{
	for(auto& Client : ClientList)
	{
		Client.Value->Init(ControllerComponent->GetOwner(), Handler, RobotName + "/" + Client.Key);
	}
}

void FRROSComunicationContainer::InitAllActionServer()
{
	for(auto& ActionServer : ActionServerList)
	{
		ActionServer.Value->Init(WebsocketIPAddr, WebsocketPort, ControllerComponent->GetOwner());
	}
}

void FRROSComunicationContainer::Tick()
{
	if(Handler.IsValid())
	{
		for(auto& Publisher : PublisherList)
		{
			Publisher.Value->Publish();
		}

		for(auto& ActionServer : ActionServerList)
		{
			ActionServer.Value->Tick();
		}

		Handler->Process();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Handler Invalid"));
	}
}

void FRROSComunicationContainer::InitHandler()
{
	if(bUseGlobalHandler)
	{
		//TODO implement use global handler
		UE_LOG(LogTemp, Error, TEXT("Global handler not supported yet"));
	}
	else
	{
		Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
	}
}

void FRROSComunicationContainer::DeInit()
{
    //Disconnect the handler before parent ends
	if(bUseGlobalHandler)
	{
		//TODO implement use global handler
		UE_LOG(LogTemp, Error, TEXT("Global handler not supported yet"));
	}
	else
	{
		for(auto& Publisher : PublisherList)
		{
			Publisher.Value->DeInit();
		}
		Handler->Disconnect();
	}

}

