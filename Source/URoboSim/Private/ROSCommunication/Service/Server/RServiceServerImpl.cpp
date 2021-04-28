#include "ROSCommunication/Service/Server/RServiceServerImpl.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSRobotRegistrationServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<unreal_controller_manager::RegisterRobot::Request> Request =
		MakeShareable(new unreal_controller_manager::RegisterRobot::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}


TSharedPtr<FROSBridgeSrv::SrvResponse> FROSRobotRegistrationServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest)
{
	return nullptr;
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSimulationCommandsServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<u_robo_sim_communication::SimulationCommands::Request> Request =
		MakeShareable(new u_robo_sim_communication::SimulationCommands::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}


TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSimulationCommandsServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest)
{
	TSharedPtr<u_robo_sim_communication::SimulationCommands::Request> Request =
		StaticCastSharedPtr<u_robo_sim_communication::SimulationCommands::Request>(InRequest);
	FString Command = Request->GetCommand();


	// Owner->CommandQuerry.Enqueue(Command);

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new u_robo_sim_communication::SimulationCommands::Response(true));
}
