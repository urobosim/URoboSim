#pragma once

#include "RServiceServer.h"
// clang-format off
#include "GetGraspedObjectServer.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URGetGraspedObjectServerParameter final : public URServiceServerParameter
{
  GENERATED_BODY()

public:
  URGetGraspedObjectServerParameter()
  {
    Type = TEXT("urobosim_msgs/get_grasped_object");
  }
};

UCLASS()
class UROBOSIM_API URGetGraspedObjectServer : public URServiceServer
{
	GENERATED_BODY()

public:
	URGetGraspedObjectServer();

protected:
	virtual void CreateServiceServer() override;
};

class FRGetGraspedObjectServerCallback : public FROSBridgeSrvServer
{
public:
	FRGetGraspedObjectServerCallback(FString Name, FString Type, UWorld *InWorld, UObject *InOwner);

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

private:
	UWorld *World;

	ARModel *Owner;
};