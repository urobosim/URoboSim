// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnRobotServer.h"
#include "WorldControlGameInstance.h"
#include "XmlFile.h"
#include "Ids.h"
#include "RobotManagerActor.h"
#include "SDFParser.h"
#include "Physics/RModel.h"
#include "Factory/RModelBuilder.h"

void USDFClient::CreateServiceClient()
{
	SDFRequest = MakeShareable(new rosapi::GetParam::Request(RobotDescription, TEXT("")));
	SDFResponse = MakeShareable(new rosapi::GetParam::Response());
	SDFParamClient = MakeShareable<FSDFParamClient>(new FSDFParamClient(RobotManager, ServiceName, ServiceType));
}

void USDFClient::CallService()
{
	Handler->CallService(SDFParamClient, SDFRequest, SDFResponse);
}

void USpawnRobotServer::CreateServiceServer()
{
	if (SDFClient)
	{
		ServiceServer = MakeShareable<FROSSpawnRobotServer>(
			new FROSSpawnRobotServer(Name, Type, GetWorld(), Controller, SDFClient));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("SDFClient not set for %s"), *GetName());
	}
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSSpawnRobotServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSRobotModelSrv::Request> Request =
		MakeShareable(new FROSRobotModelSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}


//We are first looking for the [RobotName] in Content/Robot if we find it we will look if there is a [RobotName] DataAsset and if not we will try to parse the SDF File given during the
//runtime to create an DataAsset, which is always needed to spawn a Robot. Careful even with the RuntimeParser it is still neccesarry to have all mehes already present in the
// Content/Robots/[RobotName] folder

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSSpawnRobotServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	UE_LOG(LogTemp, Log, TEXT("SpawnRobotServer recieved a Message. Time to see what it is."));
	TSharedPtr<FROSRobotModelSrv::Request> SpawnRobotRequest =
		StaticCastSharedPtr<FROSRobotModelSrv::Request>(Request);
	if (!SpawnRobotRequest)
	{
		UE_LOG(LogTemp, Log, TEXT("SpawnRobotServer recieved a Message. But in the wrong format. Aborting... "));
		return nullptr;
	}

	//Get SDF !!!!!!!!!!!!!!!!!!!!!!!!!!

	SDFClient->Id = SpawnRobotRequest->GetId().IsEmpty() ? FIds::NewGuidInBase64() : SpawnRobotRequest->GetId();
	SDFClient->CallService();

	// FString SDFFile;
	// /////////////////////////////////////////////////////////
	//
	ServiceSuccess = true;
	return MakeShareable<FROSBridgeSrv::SrvResponse>(
		new FROSRobotModelSrv::Response(SDFClient->Id, TEXT("sdf"), ServiceSuccess));;
}

AActor* FROSSpawnRobotServer::SpawnRobotFromAsset(USDFDataAsset* InDataAsset, FString InId)
{
	UE_LOG(LogTemp, Log, TEXT("[%s] RobotFactory starts to work... "), *FString(__FUNCTION__));
	USDFDataAsset* AssetToSpawn = InDataAsset;
	FString Id = InId;

	ARModel* ActortoSpawn = NewObject<ARModel>();
	USDFModel* ModeltoSpawn = AssetToSpawn->Models[0];
	// There should only be one Model in the DataAsset, if not which one should I spawn?
	FVector Position = FVector(100, -100, 20); //This does not work the robot always spawns at (0|0|0)
	FRotator Rotation = FRotator(0, 0, 0);
	FActorSpawnParameters SpawnParams;
	FString RobotName = TEXT("Robot") + FGuid::NewGuid().ToString();
	SpawnParams.Name = *RobotName;
	ActortoSpawn = World->SpawnActor<ARModel>(Position, Rotation, SpawnParams);
	//    UE_LOG(LogTemp, Log, TEXT("[%s] ActortoSpawnLocation after SpawnActor: [%f,%f,%f] "),*FString(__FUNCTION__),ActortoSpawn->GetActorLocation().X,ActortoSpawn->GetActorLocation().Y,ActortoSpawn->GetActorLocation().Z);
	ActortoSpawn->SetRootComponent(nullptr);
	//    UE_LOG(LogTemp, Log, TEXT("[%s] ActortoSpawnLocation after rootComponent=NULL: [%f,%f,%f] "),*FString(__FUNCTION__),ActortoSpawn->GetActorLocation().X,ActortoSpawn->GetActorLocation().Y,ActortoSpawn->GetActorLocation().Z);
	URModelBuilder* BuildingFacotry = NewObject<URModelBuilder>();
	BuildingFacotry->Load(ModeltoSpawn, ActortoSpawn, FVector(), AssetToSpawn->Version);
	//    UE_LOG(LogTemp, Log, TEXT("[%s] ActortoSpawnLocation after URModelBuilder->Load: [%f,%f,%f] "),*FString(__FUNCTION__),ActortoSpawn->GetActorLocation().X,ActortoSpawn->GetActorLocation().Y,ActortoSpawn->GetActorLocation().Z);

	// FTags::AddKeyValuePair(
	// 	ActortoSpawn,
	// 	TEXT("SemLog"),
	// 	TEXT("id"),
	// 	Id);


	return ActortoSpawn;
}

FSDFParamClient::FSDFParamClient(URobotManager* OutRobotManager, const FString& InName,
                                 const FString& InType): FROSBridgeSrvClient(InName, InType)
{
	RobotManager = OutRobotManager;
}

void FSDFParamClient::Callback(TSharedPtr<FROSBridgeSrv::SrvResponse> InResponse)
{
	UE_LOG(LogTemp, Log, TEXT("Receive SDF"));
	TSharedPtr<rosapi::GetParam::Response> Response =
		StaticCastSharedPtr<rosapi::GetParam::Response>(InResponse);

	TArray<TCHAR> Chars;
	Chars.Add('\n');
	Chars.Add('\"');
	FString JointString = Response->GetValue();
	JointString = JointString.ReplaceEscapedCharWithChar(&Chars);

	JointString.RemoveFromStart(TEXT("\""));
	JointString.RemoveFromEnd(TEXT("\""));

	RobotManager->SDFClient->SDFFile = JointString;
	RobotManager->ParseSDF();
}
