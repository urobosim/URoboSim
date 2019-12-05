#include "ROSCommunication/RROSClient.h"
#include "TimerManager.h"


void URROSClient::Init(UObject* InOwner, TSharedPtr<FROSBridgeHandler> InHandler)
{
	ROSHandler = InHandler;
	Init(InOwner);
}

void URROSClient::Init(UObject* InOwner, TArray<FString>* OutArray, TSharedPtr<FROSBridgeHandler> InHandler)
{
	ROSHandler = InHandler;
	Init(InOwner, OutArray);
}

void URJointStateConfigurationClient::Init(UObject* InModel, TArray<FString>* OutArray)
// void URJointStateConfigurationClient::Init(UObject* InControllerComp)
{
  ARModel* Model = Cast<ARModel>(InModel);


  Request = MakeShareable(new rosapi::GetParam::Request(JointParamTopic, ""));
  // Create an empty response instance
  Response = MakeShareable(new rosapi::GetParam::Response());
  ServiceClient = MakeShareable<FROSJointStateConfigurationClient>(new FROSJointStateConfigurationClient(OutArray,TEXT("rosapi/get_param"), TEXT("rosapi/GetParam")));
  FTimerHandle MyTimerHandle;
  Model->GetWorldTimerManager().SetTimer(MyTimerHandle, this, &URJointStateConfigurationClient::CallService, 1.0f, false);
}

void URJointStateConfigurationClient::CallService()
{

  ROSHandler->CallService(ServiceClient, Request, Response);

}

void URJointControllerConfigurationClient::Init(UObject* InControllerComp)
{
  // ARModel* Model = Cast<ARModel>(InModel);
  ControllerComp = Cast<URControllerComponent>(InControllerComp);
  CreateClient();
  // ServiceClient = MakeShareable<FROSJointStateConfigurationClient>(new FROSJointStateConfigurationClient(OutArray,TEXT("rosapi/get_param"), TEXT("rosapi/GetParam")));
}

void URJointControllerConfigurationClient::CreateClient()
{
  URJointController* JointController = Cast<URJointController>(ControllerComp->ControllerList("JointController"));
  TMap<FString,float>* JointNames = &JointController->DesiredJointState;
  Request = MakeShareable(new rosapi::GetParam::Request(JointParamTopic, ""));
  // Create an empty response instance
  Response = MakeShareable(new rosapi::GetParam::Response());
  ServiceClient = MakeShareable<FROSJointControllerConfigurationClient>(new FROSJointControllerConfigurationClient(JointNames,TEXT("rosapi/get_param"), TEXT("rosapi/GetParam")));
  FTimerHandle MyTimerHandle;
  ControllerComp->GetOwner()->GetWorldTimerManager().SetTimer(MyTimerHandle, this, &URJointControllerConfigurationClient::CallService, 1.0f, false);
}

void URJointControllerConfigurationClient::CallService()
{

  ROSHandler->CallService(ServiceClient, Request, Response);

}
