#pragma once

#include "RServiceServer.h"
// clang-format off
#include "RSimulationCommandsServer.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URSimulationCommandsServerParameter final : public URServiceServerParameter
{
  GENERATED_BODY()

public:
  URSimulationCommandsServerParameter()
  {
    Type = TEXT("u_robo_sim_communication/SimulationCommands");
  }
};

UCLASS()
class UROBOSIM_API URSimulationCommandsServer final : public URServiceServer
{
	GENERATED_BODY()
  
public:
  URSimulationCommandsServer();

protected:
	void CreateServiceServer() override;
};

class FRSimulationCommandsServerCallback final : public FROSBridgeSrvServer
{

public:
	FRSimulationCommandsServerCallback(const FString& InName, const FString& InType);

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest) override;
};