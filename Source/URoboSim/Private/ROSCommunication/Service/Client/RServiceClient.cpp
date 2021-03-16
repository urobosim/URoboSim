#include "ROSCommunication/Service/Client/RServiceClient.h"
#include "Controller/RController.h"
#include "TimerManager.h"

void URServiceClient::Init(UObject *InControllerComp, TSharedPtr<FROSBridgeHandler> InHandler)
{
  Handler = InHandler;
  Init(InControllerComp);
}

void URServiceClient::Init(UObject *InControllerComp, TArray<FString> *OutArray, TSharedPtr<FROSBridgeHandler> InHandler)
{
  Handler = InHandler;
  Init(InControllerComp, OutArray);
}

// URJointControllerConfigurationClient::URJointControllerConfigurationClient()
// {
//   JointParamTopic = TEXT("/whole_body_controller/joints");
//   LimitParamTopic = TEXT("/robot_description");
// }

// void URJointControllerConfigurationClient::Init(UObject *InControllerComp)
// {
//   // ARModel* Model = Cast<ARModel>(InModel);
//   ControllerComp = Cast<URControllerComponent>(InControllerComp);
//   CreateClient();
//   // ServiceClient = MakeShareable<FROSJointStateConfigurationClient>(new FROSJointStateConfigurationClient(OutArray,TEXT("rosapi/get_param"), TEXT("rosapi/GetParam")));
// }

// void URJointControllerConfigurationClient::CreateClient()
// {
//   URJointController *JointController = Cast<URJointController>(ControllerComp->ControllerList("JointController"));
//   TMap<FString, float> *JointNames = &JointController->DesiredJointState;
//   JointRequest = MakeShareable(new rosapi::GetParam::Request(JointParamTopic, ""));
//   JointResponse = MakeShareable(new rosapi::GetParam::Response());
//   JointServiceClient = MakeShareable<FROSJointControllerConfigurationClient>(new FROSJointControllerConfigurationClient(JointNames, TEXT("rosapi/get_param"), TEXT("rosapi/GetParam")));

//   LimitRequest = MakeShareable(new rosapi::GetParam::Request(LimitParamTopic, ""));
//   LimitResponse = MakeShareable(new rosapi::GetParam::Response());
//   JointLimitServiceClient = MakeShareable<FROSJointLimitControllerConfigurationClient>(new FROSJointLimitControllerConfigurationClient(JointNames, ControllerComp->GetOwner(), TEXT("rosapi/get_param"), TEXT("rosapi/GetParam")));

//   FTimerHandle MyTimerHandle;
//   ControllerComp->GetOwner()->GetWorldTimerManager().SetTimer(MyTimerHandle, this, &URJointControllerConfigurationClient::CallService, 1.0f, false);
// }

// void URJointControllerConfigurationClient::CallService()
// {
//   ROSHandler->CallService(JointServiceClient, JointRequest, JointResponse);
//   ROSHandler->CallService(JointLimitServiceClient, LimitRequest, LimitResponse);
// }