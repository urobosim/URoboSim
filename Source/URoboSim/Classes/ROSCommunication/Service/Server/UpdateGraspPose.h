#pragma once

#include "RServiceServer.h"
// clang-format off
#include "UpdateGraspPose.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URUpdateGraspPoseServerParameter final : public URServiceServerParameter
{
  GENERATED_BODY()

public:
  URUpdateGraspPoseServerParameter()
  {
    Type = TEXT("world_control_msgs/SetModelPose");
  }
};

UCLASS()
class UROBOSIM_API URUpdateGraspPoseServer : public URServiceServer
{
	GENERATED_BODY()

public:
	URUpdateGraspPoseServer();

protected:
	virtual void CreateServiceServer() override;
};

class FRUpdateGraspPoseServerCallback : public FROSBridgeSrvServer
{
public:
	FRUpdateGraspPoseServerCallback(FString Name, FString Type, UWorld *InWorld, UObject *InOwner);

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

private:
	UWorld *World;

	ARModel *Owner;
};
