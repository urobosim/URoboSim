#include "ROSCommunication/CheckVisibilityService.h"
#include "ROSCommunication/CheckVisibilityServiceServer.h"

void UCheckVisibilityService::Init()
{
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  Handler->Connect();
  Server = MakeShareable<FROSCheckVisebilityServer>(new FROSCheckVisebilityServer(TEXT("unreal"), TEXT("move_object"), GetWorld(), this));
  Handler->AddServiceServer(Server);
}
