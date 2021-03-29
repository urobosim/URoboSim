#include "ROSCommunication/Service/Client/RGetJointsClient.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGetJointsClient, Log, All);

URGetJointsClient::URGetJointsClient() : URGetParamClient::URGetParamClient()
{
  GetParamArgument.Name = TEXT("hardware_interface/joints");
}

void URGetJointsClient::Init()
{
  if (GetOwner())
  {
    ControllerComponent = Cast<URControllerComponent>(GetOwner()->GetPlugin(TEXT("ControllerComponent")));

    if (ControllerComponent)
    {
      // Create the service client
      GetParamClient = MakeShareable<FRGetJointsClientCallback>(new FRGetJointsClientCallback(ServiceName, ServiceType, ControllerComponent->GetController(TEXT("JointController"))));
    }

    // Create a request instance with request parameters
    Request = MakeShareable(new rosapi::GetParam::Request(GetParamArgument.Name, GetParamArgument.Default));

    // Create an empty response instance
    Response = MakeShareable(new rosapi::GetParam::Response());

    FTimerHandle MyTimerHandle;
    GetOwner()->GetWorldTimerManager().SetTimer(MyTimerHandle, this, &URGetJointsClient::CallService, 1.f, false);
  }
}

FRGetJointsClientCallback::FRGetJointsClientCallback(const FString &InServiceName, const FString &InServiceType, UObject *InController) : FRGetParamClientCallback::FRGetParamClientCallback(InServiceName, InServiceType, InController)
{
}

void FRGetJointsClientCallback::Callback()
{
  ParamString.RemoveFromStart(TEXT("["));
  ParamString.RemoveFromEnd(TEXT("]"));
  
  UE_LOG(LogRGetJointsClient, Log, TEXT("ParamString: %s"), *ParamString)
  TArray<FString> JointsStringArray;
  ParamString.ParseIntoArray(JointsStringArray, TEXT(","), true);
  for (FString &JointsString : JointsStringArray)
  {
    JointsString = JointsString.TrimStartAndEnd().TrimQuotes();
  }
  if (JointController)
  {
    JointController->SetJointNames(JointsStringArray);
  }
  else
  {
    UE_LOG(LogRGetJointsClient, Error, TEXT("JointController not found"))
  }
}