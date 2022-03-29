#pragma once

#include "RServiceClient.h"
#include "ROSBridgeSrvClient.h"
#include "json_prolog_msgs/srv/PrologQuery.h"
// clang-format off
#include "PrologQueryClient.generated.h"
// clang-format on

UCLASS()
class UROBOSIM_API UPrologQueryClientParameter final : public URServiceClientParameter
{
  GENERATED_BODY()

public:
  UPrologQueryClientParameter()
  {
    ServiceName = TEXT("rosprolog/query");
    ServiceType = TEXT("json_prolog_msgs/PrologQuery");
  }

public:
};

UCLASS()
class UROBOSIM_API UPrologQueryClient final : public UServiceClient
{
  GENERATED_BODY()

public:
  UPrologQueryClient();

public:
  void SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters) override;

  void CallService() override;

  // uint Mode;
  // FString Query;
  // FString Id;
  bool bOk;
  FString Message;
protected:
  void CreateServiceClient() override;

public:
  void SendQuery(const uint8 InMode, const FString& InId, const FString& InQuery, FTimerManager& InTimerManager);

public:

private:
  TSharedPtr<class FPrologQueryClient> ServiceClient;

  TSharedPtr<json_prolog_msgs::PrologQuerySrv::Request> Request;

  TSharedPtr<json_prolog_msgs::PrologQuerySrv::Response> Response;
};

class FPrologQueryClient final : public FROSBridgeSrvClient
{
public:
  FPrologQueryClient(const FString &InName, const FString &InType, const FString &InQuery);

  void Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) override;

private:
    uint32 Mode;
    FString Query;
    FString Id;
};
