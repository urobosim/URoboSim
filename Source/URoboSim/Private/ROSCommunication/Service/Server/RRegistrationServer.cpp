#include "ROSCommunication/Service/Server/RRegistrationServer.h"
#include "PR2UnrealSimulator/RegisterRobot.h"

URRegistrationServer::URRegistrationServer()
{
  Type = TEXT("unreal_controller_manager/RegisterRobot");
}

void URRegistrationServer::CreateServiceServer()
{
  ServiceServer = MakeShareable<FRRegistrationServerCallback>(new FRRegistrationServerCallback(Name, Type));
}

FRRegistrationServerCallback::FRRegistrationServerCallback(const FString &InName, const FString &InType) : FROSBridgeSrvServer(InName, InType)
{
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FRRegistrationServerCallback::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<unreal_controller_manager::RegisterRobot::Request> Request =
      MakeShareable(new unreal_controller_manager::RegisterRobot::Request());
  Request->FromJson(JsonObject);
  return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FRRegistrationServerCallback::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest)
{
  return nullptr;
}