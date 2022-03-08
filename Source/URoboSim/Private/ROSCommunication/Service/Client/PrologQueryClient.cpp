#include "ROSCommunication/Service/Client/PrologQueryClient.h"

UPrologQueryClient::UPrologQueryClient()
{
    ServiceName = TEXT("rosprolog/query");
    ServiceType = TEXT("json_prolog_msgs/PrologQuery");
}

void UPrologQueryClient::SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters)
{
  if (UPrologQueryClientParameter *ClientParameter = Cast<UPrologQueryClientParameter>(ServiceClientParameters))
  {
    Super::SetServiceClientParameters(ServiceClientParameters);
  }
}

void UPrologQueryClient::CreateServiceClient()
{
  // Request = MakeShareable(new json_prolog_msgs::PrologQuerySrv::Request(std_msgs::Byte(Mode), Id, Query));
  //   // Create an empty response instance
  //   Response = MakeShareable(new json_prolog_msgs::PrologQuerySrv::Response());
}

void UPrologQueryClient::SendQuery(const uint8 InMode, const FString& InId, const FString& InQuery, FTimerManager& InTimerManager)
{
    ServiceClient = MakeShareable<FPrologQueryClient>(new FPrologQueryClient(ServiceName, ServiceType, InQuery));

    Request = MakeShareable(new json_prolog_msgs::PrologQuerySrv::Request(std_msgs::Byte(InMode), InId, InQuery));
    // Create an empty response instance
    Response = MakeShareable(new json_prolog_msgs::PrologQuerySrv::Response(bOk, Message));

    FTimerHandle MyTimerHandle;
    InTimerManager.SetTimer(MyTimerHandle, this, &UPrologQueryClient::CallService, 1.0f, false);
}

void UPrologQueryClient::CallService()
{
  Handler->CallService(ServiceClient, Request, Response);
}

FPrologQueryClient::FPrologQueryClient(const FString &InName, const FString &InType, const FString &InQuery) : FROSBridgeSrvClient(InName, InType)
{
  Query = InQuery;
}

void FPrologQueryClient::Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{
  TSharedPtr<json_prolog_msgs::PrologQuerySrv::Response> Response =
      StaticCastSharedPtr<json_prolog_msgs::PrologQuerySrv::Response>(InResponse);

}
