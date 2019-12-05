
#include "ROSCommunication/RROSClientImpl.h"
#include "ROSCommunication/RRosComunication.h"
// #include "ROSCommunication/RPublisher.h"
#include "ROSCommunication/RROSClient.h"
// #include "Controller/RController.h"
// #include "ROSCommunication/RRosCommunicationComponent.h"
#include "Physics/RModel.h"


void FROSJointStateConfigurationClient::Callback( TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{

	TSharedPtr<rosapi::GetParam::Response> Response =
		StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

	FString JointString = Response->GetValue();
	TArray<FString> StringArray;
	JointString.RemoveFromStart(TEXT("["));
	JointString.RemoveFromEnd(TEXT("]"));
	JointString.ParseIntoArray(StringArray,TEXT(","),true);
	for(auto& st : StringArray)
	{
		st = st.Trim().TrimQuotes();
	}
	JointNames->Empty();
	JointNames->Append(StringArray);
}

FROSJointStateConfigurationClient::FROSJointStateConfigurationClient(TArray<FString>* OutJointNames, const FString& InName, const FString& InType):
	FROSBridgeSrvClient(InName, InType)
{
	JointNames = OutJointNames;
}

void FROSJointControllerConfigurationClient::Callback( TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{

	TSharedPtr<rosapi::GetParam::Response> Response =
		StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

	FString JointString = Response->GetValue();
	TArray<FString> StringArray;
	JointString.RemoveFromStart(TEXT("["));
	JointString.RemoveFromEnd(TEXT("]"));
	JointString.ParseIntoArray(StringArray,TEXT(","),true);
	for(auto& st : StringArray)
	{
		st = st.Trim().TrimQuotes();
          float& JointState = JointNames->FindOrAdd(st);
          JointState = 0.f;
	}
}

FROSJointControllerConfigurationClient::FROSJointControllerConfigurationClient(TMap<FString, float>* OutJointNames, const FString& InName, const FString& InType):
	FROSBridgeSrvClient(InName, InType)
{
	JointNames = OutJointNames;
}
