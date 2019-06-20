#include "ROSCommunication/RROSClient.h"
#include "TimerManager.h"


void URROSClient::Init(UObject* InModel, TSharedPtr<FROSBridgeHandler> InHandler, FString InName)
{
	ROSHandler = InHandler;
	// Name = InName;
	Init(InModel);
}

void URROSClient::Init(UObject* InModel, TArray<FString>* OutArray, TSharedPtr<FROSBridgeHandler> InHandler)
{
	ROSHandler = InHandler;
	Init(InModel, OutArray);
}

void URJointStateConfigurationClient::Init(UObject* InModel, TArray<FString>* OutArray)
{
	ARModel* Model = Cast<ARModel>(InModel);

	ServiceClient = MakeShareable<FROSJointStateConfigurationClient>(new FROSJointStateConfigurationClient(OutArray,TEXT("rosapi/get_param"), TEXT("rosapi/GetParam")));
	FTimerHandle MyTimerHandle;
	Model->GetWorldTimerManager().SetTimer(MyTimerHandle, this, &URJointStateConfigurationClient::CallService, 1.0f, false);
	// UE_LOG(LogTemp, Error, TEXT("befor call"));
	// CallService();
	// UE_LOG(LogTemp, Error, TEXT("after call"));
}

void URJointStateConfigurationClient::CallService()
{
	// Create a request instance with request parameters
	Request = MakeShareable(new rosapi::GetParam::Request(JointParamTopic, ""));
	// Create an empty response instance
	Response = MakeShareable(new rosapi::GetParam::Response());

	ROSHandler->CallService(ServiceClient, Request, Response);

}
