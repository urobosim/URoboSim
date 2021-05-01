#pragma once

#include "RServiceServer.h"
// clang-format off
#include "CheckVisibilityServer.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URCheckVisibilityServerParameter final : public URServiceServerParameter
{
  GENERATED_BODY()

public:
  URCheckVisibilityServerParameter()
  {
    Type = TEXT("urobosim_msgs/CheckVisibility");
  }
};

UCLASS()
class UROBOSIM_API URCheckVisibilityServer : public URServiceServer
{
	GENERATED_BODY()

public:
	URCheckVisibilityServer();

protected:
	virtual void CreateServiceServer() override;
};

class FRCheckVisibilityServerCallback : public FROSBridgeSrvServer
{
public:
	FRCheckVisibilityServerCallback(FString Name, FString Type, UWorld *InWorld, UObject *InOwner);

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

private:
	UWorld *World;

	ARModel *Owner;

	int32 PlayerIndex;

	FThreadSafeBool bAllSuccessfull;
};