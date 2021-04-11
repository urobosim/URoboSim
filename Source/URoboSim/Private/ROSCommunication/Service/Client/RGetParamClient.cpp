#include "ROSCommunication/Service/Client/RGetParamClient.h"
#include "TimerManager.h"

URGetParamClient::URGetParamClient()
{
  ServiceName = TEXT("rosapi/get_param");
  ServiceType = TEXT("rosapi/GetParam");
}

void URGetParamClient::SetServiceClientParameters(URServiceClientParameter *&ServiceClientParameters)
{
  Super::SetServiceClientParameters(ServiceClientParameters);
  URGetParamClientParameter *GetParamClientParameter = Cast<URGetParamClientParameter>(ServiceClientParameters);
  if (GetParamClientParameter)
  {
    GetParamArguments = GetParamClientParameter->GetParamArguments;
  }  
}

void URGetParamClient::Init()
{
  if (GetOwner())
  {
    // Create a request instance with request parameters
    Request = MakeShareable(new rosapi::GetParam::Request(GetParamArguments.Name, GetParamArguments.Default));

    // Create an empty response instance
    Response = MakeShareable(new rosapi::GetParam::Response());
  }
}

void URGetParamClient::CallService()
{
  Handler->CallService(GetParamClient, Request, Response);
}

FRGetParamClientCallback::FRGetParamClientCallback(const FString &InServiceName, const FString &InServiceType, URController *InController) : FROSBridgeSrvClient(InServiceName, InServiceType)
{
  Controller = InController;
}

void FRGetParamClientCallback::Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{
  TSharedPtr<rosapi::GetParam::Response> Response =
      StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

  ParamString = Response->GetValue();
  Callback();
}