#include "ROSCommunication/RROSCommunication.h"
#include "ROSCommunication/RROSCommunicationComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRROSCommunication, Log, All);

void URROSCommunication::Connect(const TSharedPtr<FROSBridgeHandler> &InHandler)
{
  Handler = InHandler;
  if (!Handler.IsValid())
  {
    UE_LOG(LogRROSCommunication, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
  SetOwner();
  Init();
}

void URROSCommunication::Connect(const FString &WebsocketIPAddr, const uint32 &WebsocketPort)
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  if (Handler.IsValid())
  {
    Handler->Connect();
    UE_LOG(LogRROSCommunication, Log, TEXT("%s is connected to ROSBridge"), *GetName())
  }
  else
  {
    UE_LOG(LogRROSCommunication, Error, TEXT("No FROSBridgeHandler created in %s"), *GetName())
  }
  SetOwner();
  Init();
}

void URROSCommunication::Disconnect()
{
  if (Handler.IsValid())
  {
    Handler->Disconnect();
  }
}

void URROSCommunication::SetOwner()
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
    UE_LOG(LogRROSCommunication, Error, TEXT("Owner of %s not found, Outer is %s"), *GetName(), *GetOuter()->GetName())
  }
}