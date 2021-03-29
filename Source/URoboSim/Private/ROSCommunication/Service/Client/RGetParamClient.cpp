#include "ROSCommunication/Service/Client/RGetParamClient.h"
#include "TimerManager.h"

URGetParamClient::URGetParamClient()
{
  ServiceName = TEXT("rosapi/get_param");
  ServiceType = TEXT("rosapi/GetParam");
}

void URGetParamClient::Init()
{
  if (GetOwner())
  {
    ControllerComponent = Cast<URControllerComponent>(GetOwner()->GetPlugin(TEXT("ControllerComponent")));

    if (ControllerComponent)
    {
      // Create the service client
      GetParamClient = MakeShareable<FRGetParamClientCallback>(new FRGetParamClientCallback(ServiceName, ServiceType, ControllerComponent->GetController(TEXT("JointController"))));
    }

    // Create a request instance with request parameters
    Request = MakeShareable(new rosapi::GetParam::Request(GetParamArgument.Name, GetParamArgument.Default));

    // Create an empty response instance
    Response = MakeShareable(new rosapi::GetParam::Response());

    FTimerHandle MyTimerHandle;
    GetOwner()->GetWorldTimerManager().SetTimer(MyTimerHandle, this, &URGetParamClient::CallService, 1.f, false);
  }
}

void URGetParamClient::CallService()
{
  Handler->CallService(GetParamClient, Request, Response);
}

FRGetParamClientCallback::FRGetParamClientCallback(const FString &InServiceName, const FString &InServiceType, UObject *InController) : FROSBridgeSrvClient(InServiceName, InServiceType)
{
  JointController = Cast<URJointController>(InController);
}

void FRGetParamClientCallback::Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{
  TSharedPtr<rosapi::GetParam::Response> Response =
      StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

  ParamString = Response->GetValue();
  Callback();
}