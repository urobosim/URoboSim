
#include "ROSCommunication/RROSClientImpl.h"
#include "ROSCommunication/RRosComunication.h"
#include "ROSCommunication/RPublisher.h"
#include "ROSCommunication/RROSClient.h"
#include "Controller/RController.h"
#include "ROSCommunication/RRosCommunicationComponent.h"
#include "Physics/RModel.h"



/* void FROSRobotRegistrationClient::Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) */
/* { */
/* 	TSharedPtr<unreal_controller_manager::RegisterRobot::Response> Response = */
/* 		StaticCastSharedPtr<unreal_controller_manager::RegisterRobot::Response>(InResponse); */
/*  */
/* 	Client->bIsRegisterd = Response->GetSuccess(); */
/* } */

/* FROSUpdateWorldClient::FROSUpdateWorldClient(UObject* InModel, const FString& InName, const FString& InType): */
/* 	FROSBridgeSrvClient(InName, InType) */
/* { */
/* 	ControllerComponent = Cast<URControllerComponent>(Cast<ARModel>(InModel)->FindComponentByClass(URControllerComponent::StaticClass())); */
/* 	BaseController= Cast<URBaseController>(ControllerComponent->ControllerList("BaseController")); */
/* } */
/*  */
/* void FROSUpdateWorldClient::Callback( TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse) */
/* { */
/*  */
/* 	TSharedPtr<unreal_controller_manager::UpdateWorld::Response> Response = */
/* 		StaticCastSharedPtr<unreal_controller_manager::UpdateWorld::Response>(InResponse); */
/*  */
/* 	if(ControllerComponent) */
/* 	{ */
/* 		ControllerComponent->SetJointVelocities(Response->GetJointNames(), Response->GetJointVelocities()); */
/* 		BaseController->Turn(Response->GetBaseControllCommand().GetAngular().GetZ()); */
/* 		BaseController->MoveLinear(Response->GetBaseControllCommand().GetLinear().GetVector()); */
/*  */
/* 		// ControllerComponent->ExcecuteCommands(Response->GetCommands()); */
/* 		TArray<FString> ResponseCommands = Response->GetCommands(); */
/* 		for(auto& Command : ResponseCommands) */
/* 		{ */
/* 			ControllerComponent->CommandQuerry.Enqueue(Command); */
/* 		} */
/* 	} */
/* 	else */
/* 	{ */
/* 		UE_LOG(LogTemp, Error, TEXT("Model is not of type ARModel")); */
/* 	} */
/* } */

void FROSJointStateConfigurationClient::Callback( TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{

	TSharedPtr<rosapi::GetParam::Response> Response =
		StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

	FString JointString = Response->GetValue();
	TArray<FString> StringArray;
	JointString.RemoveFromStart(TEXT("["));
	JointString.RemoveFromEnd(TEXT("]"));
	JointString.ParseIntoArray(StringArray,TEXT(","),true);
	UE_LOG(LogTemp, Error, TEXT("JointNames Callback"));
	for(auto& st : StringArray)
	{
		st = st.Trim().TrimQuotes();
		// UE_LOG(LogTemp, Log, TEXT("%"), *st);
	}
	// JSPublisher->ListJointName = StringArray;
	JointNames->Empty();
	JointNames->Append(StringArray);
}

FROSJointStateConfigurationClient::FROSJointStateConfigurationClient(TArray<FString>* OutJointNames, const FString& InName, const FString& InType):
	FROSBridgeSrvClient(InName, InType)
{
	//TODO make jspublisher variable

	JointNames = OutJointNames;


	// URROSCommunicationComponent* ComComp = Cast<URROSCommunicationComponent>(Cast<ARModel>(InOwner)->FindComponentByClass(URROSCommunicationComponent::StaticClass()));
	/* if(ComComp) */
	/* { */
	/* 	JSPublisher = Cast<URJointStatePublisher>(ComComp->RosComunication.PublisherList["JointState"]); */
	/* 	if(!JSPublisher) */
	/* 	{ */
	/* 		UE_LOG(LogTemp, Error, TEXT("JSPublisher not found")); */
	/* 	} */
	/* } */
	/* else */
	/* { */
	/* 	UE_LOG(LogTemp, Error, TEXT("No Comunication Component found")); */
	/* } */
}
