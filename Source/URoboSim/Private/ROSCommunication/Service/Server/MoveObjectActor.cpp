#include "ROSCommunication/Service/Server/MoveObjectActor.h"
#include "ROSCommunication/Service/Server/MoveObjectServiceServer.h"

void AMoveObjectActor::BeginPlay()
{
  Super::BeginPlay();
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  Handler->Connect();
  Server = MakeShareable<FROSMoveObjectServer>(new FROSMoveObjectServer(TEXT("unreal"), TEXT("move_object"), GetWorld(), this));
  Handler->AddServiceServer(Server);
}

void AMoveObjectActor::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
  Handler->Process();
}

void AMoveObjectActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Handler->Disconnect();
}
