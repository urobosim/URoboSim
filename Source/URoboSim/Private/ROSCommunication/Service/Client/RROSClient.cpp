#include "ROSCommunication/Service/Client/RROSClient.h"
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

URJointControllerConfigurationClient::URJointControllerConfigurationClient()
{
  JointParamTopic = TEXT("/whole_body_controller/joints");
  LimitParamTopic = TEXT("/robot_description");
}

void URJointControllerConfigurationClient::SetROSClientParameters(URROSClientParameter *&ROSClientParameters)
{
  Super::SetROSClientParameters(ROSClientParameters);
  URJointControllerConfigurationClientParameter *JointControllerConfigurationClientParameters = Cast<URJointControllerConfigurationClientParameter>(ROSClientParameters);
  if (JointControllerConfigurationClientParameters)
  {
    JointParamTopic = JointControllerConfigurationClientParameters->JointParamTopic;
    LimitParamTopic = JointControllerConfigurationClientParameters->LimitParamTopic;
  }  
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
  URJointController* JointController = Cast<URJointController>(ControllerComp->GetController(TEXT("JointController")));
  TMap<FString,float>* JointNames = &JointController->DesiredJointState;
  JointRequest = MakeShareable(new rosapi::GetParam::Request(JointParamTopic, ""));
  JointResponse = MakeShareable(new rosapi::GetParam::Response());
  JointServiceClient = MakeShareable<FROSJointControllerConfigurationClient>(new FROSJointControllerConfigurationClient(JointController,JointNames,TEXT("rosapi/get_param"), TEXT("rosapi/GetParam")));

  LimitRequest = MakeShareable(new rosapi::GetParam::Request(LimitParamTopic, ""));
  LimitResponse = MakeShareable(new rosapi::GetParam::Response());
  JointLimitServiceClient = MakeShareable<FROSJointLimitControllerConfigurationClient>(new FROSJointLimitControllerConfigurationClient( JointNames, ControllerComp->GetOwner(), TEXT("rosapi/get_param"), TEXT("rosapi/GetParam")));


  FTimerHandle MyTimerHandle;
  ControllerComp->GetOwner()->GetWorldTimerManager().SetTimer(MyTimerHandle, this, &URJointControllerConfigurationClient::CallService, 1.0f, false);
}

void URJointControllerConfigurationClient::CallService()
{
  ROSHandler->CallService(JointServiceClient, JointRequest, JointResponse);
  ROSHandler->CallService(JointLimitServiceClient, LimitRequest, LimitResponse);
}
