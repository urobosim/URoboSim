#include "ROSCommunication/Service/Client/RServiceClient.h"

DEFINE_LOG_CATEGORY_STATIC(LogRServiceClient, Log, All)

void URServiceClient::Init(UObject *InOwner, const TSharedPtr<FROSBridgeHandler> &InHandler)
{
  Handler = InHandler;
  if (Handler.IsValid())
  {
    Init(InOwner);
  }
  else
  {
    UE_LOG(LogRServiceClient, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URServiceClient::Init(UObject *InOwner, const FString &WebsocketIPAddr, const uint32 &WebsocketPort)
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  if (Handler.IsValid())
  {
    Handler->Connect();
    Init(InOwner);
  }
  else
  {
    UE_LOG(LogRServiceClient, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
}

void URServiceClient::Init(UObject *InOwner)
{
  SetOwner(InOwner);
  Init();
}

void URServiceClient::Tick()
{
  if (Handler.IsValid())
  {
    Handler->Process();
  }
}