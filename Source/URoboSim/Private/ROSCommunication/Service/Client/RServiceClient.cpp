#include "ROSCommunication/Service/Client/RServiceClient.h"
#include "ROSCommunication/RROSCommunicationComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRServiceClient, Log, All)

void URServiceClient::Init(const TSharedPtr<FROSBridgeHandler> &InHandler)
{
  Handler = InHandler;
  if (Handler.IsValid())
  {
    Init();
  }
  else
  {
    UE_LOG(LogRServiceClient, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URServiceClient::Init(const FString &WebsocketIPAddr, const uint32 &WebsocketPort)
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  if (Handler.IsValid())
  {
    Handler->Connect();
    UE_LOG(LogRServiceClient, Log, TEXT("%s is connected to ROSBridge"), *GetName())
    Init();
  }
  else
  {
    UE_LOG(LogRServiceClient, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URServiceClient::Init()
{
  SetOwner();
}

void URServiceClient::SetOwner()
{
  if (!Owner)
  {
    if (Cast<ARModel>(GetOuter()))
    {
      Owner = Cast<ARModel>(GetOuter());
    }
    else if (Cast<URROSCommunicationComponent>(GetOuter()) && Cast<ARModel>(Cast<URROSCommunicationComponent>(GetOuter())->GetOwner()))
    {
      Owner = Cast<ARModel>(Cast<URROSCommunicationComponent>(GetOuter())->GetOwner());
    }
  }
  if (!Owner)
  {
    UE_LOG(LogRServiceClient, Error, TEXT("Owner of %s not found, Outer is %s"), *GetName(), *GetOuter()->GetName())
  }
}

void URServiceClient::DeInit()
{
  if (Handler.IsValid())
  {
    Handler->Disconnect();
  }
}

void URServiceClient::Tick()
{
  if (Handler.IsValid())
  {
    Handler->Process();
  }
}

void URServiceClient::SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters)
{
  if (ServiceClientParameters)
  {
    ServiceName = ServiceClientParameters->ServiceName;
    ServiceType = ServiceClientParameters->ServiceType;
  }
}