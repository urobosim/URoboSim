#include "ROSCommunication/Service/Server/RSimulationCommandsServer.h"
#include "srv/SimulationCommands.h"

URSimulationCommandsServer::URSimulationCommandsServer()
{
  Type = TEXT("u_robo_sim_communication/SimulationCommands");
}

void URSimulationCommandsServer::CreateServiceServer()
{
  ServiceServer = MakeShareable<FRSimulationCommandsServerCallback>(new FRSimulationCommandsServerCallback(Name, Type));
}

FRSimulationCommandsServerCallback::FRSimulationCommandsServerCallback(const FString &InName, const FString &InType) : FROSBridgeSrvServer(InName, InType)
{
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FRSimulationCommandsServerCallback::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<u_robo_sim_communication::SimulationCommands::Request> Request =
      MakeShareable(new u_robo_sim_communication::SimulationCommands::Request());
  Request->FromJson(JsonObject);
  
  return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FRSimulationCommandsServerCallback::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest)
{
  TSharedPtr<u_robo_sim_communication::SimulationCommands::Request> Request =
      StaticCastSharedPtr<u_robo_sim_communication::SimulationCommands::Request>(InRequest);
  FString Command = Request->GetCommand();

  // Owner->CommandQuerry.Enqueue(Command); is this needed?

  return MakeShareable<FROSBridgeSrv::SrvResponse>(new u_robo_sim_communication::SimulationCommands::Response(true));
}
