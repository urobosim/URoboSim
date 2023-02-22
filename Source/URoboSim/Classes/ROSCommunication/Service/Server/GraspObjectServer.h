#pragma once

#include "RServiceServer.h"
// clang-format off
#include "GraspObjectServer.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URGraspObjectServerParameter final : public URServiceServerParameter
{
  GENERATED_BODY()

public:
  URGraspObjectServerParameter()
  {
    Type = TEXT("urobosim_msgs/GraspObject");
  }

  UPROPERTY(EditAnywhere)
  FString ControllerName = TEXT("RGripperController");
};

UCLASS()
class UROBOSIM_API URGraspObjectServer : public URServiceServer
{
	GENERATED_BODY()

public:
	URGraspObjectServer();

	virtual void SetServiceServerParameters(URServiceServerParameter*& ServiceServerParameters) override;
protected:
	virtual void CreateServiceServer() override;

        UPROPERTY()
        URGripperControllerBase* Controller = nullptr;

        UPROPERTY(VisibleAnywhere)
        FString ControllerName = TEXT("RGripperController");
};

class FRGraspObjectServerCallback : public FROSBridgeSrvServer
{
public:
	FRGraspObjectServerCallback(FString Name, FString Type, UWorld *InWorld, URGripperControllerBase *InController);

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;

private:


        UWorld *World;

        UPROPERTY()
        URGripperControllerBase* Controller = nullptr;

	ARModel *Owner;

	int32 PlayerIndex;

	FThreadSafeBool bAllSuccessfull;
};
