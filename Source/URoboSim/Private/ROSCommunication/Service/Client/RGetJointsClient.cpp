#include "ROSCommunication/Service/Client/RGetJointsClient.h"
#include "TimerManager.h"
#include "Controller/ControllerType/RJointController.h"

DEFINE_LOG_CATEGORY_STATIC(LogRGetJointsClient, Log, All);

URGetJointsClient::URGetJointsClient() : URGetParamClient::URGetParamClient()
{
  GetParamArguments.Name = TEXT("hardware_interface/joints");
  ControllerName = TEXT("JointController");
}

void URGetJointsClient::Init()
{
  Super::Init();
  if (GetOwner())
  {
    // Create the service client
    GetParamClient = MakeShareable<FRGetJointsClientCallback>(new FRGetJointsClientCallback(ServiceName, ServiceType, GetOwner()->GetController(ControllerName)));

    FTimerHandle MyTimerHandle;
    GetOwner()->GetWorldTimerManager().SetTimer(MyTimerHandle, this, &URGetJointsClient::CallService, 1.f, false);
  }
}

FRGetJointsClientCallback::FRGetJointsClientCallback(const FString &InServiceName, const FString &InServiceType, URController *InController) : FRGetParamClientCallback::FRGetParamClientCallback(InServiceName, InServiceType, InController)
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

  static URJointController *JointController;
  JointController = Cast<URJointController>(Controller);

  if (JointController)
  {
    JointController->SetJointNames(JointsStringArray);
  }
  else
  {
    UE_LOG(LogRGetJointsClient, Error, TEXT("JointController not found"))
  }
}