#pragma once

#include "RServiceServer.h"
// clang-format off
#include "RRegistrationServer.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API URRegistrationServer final : public URServiceServer
{
  GENERATED_BODY()

protected:
  void CreateServiceServer() override;
};

class FRRegistrationServerCallback final : public FROSBridgeSrvServer
{
public:
  FRRegistrationServerCallback(const FString &InName, const FString &InType);

  TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

  TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> InRequest) override;
};