
// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "ROSCommunication/SpawnRobotGhost.h"
#include "Conversions.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"


TSharedPtr<FROSBridgeSrv::SrvRequest> FSpawnRobotGhostServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FSpawnRobotGhostSrv::Request> Request =
		MakeShareable(new FSpawnRobotGhostSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FSpawnRobotGhostServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
        TSharedPtr<FSpawnRobotGhostSrv::Request> MoveRequest =
		StaticCastSharedPtr<FSpawnRobotGhostSrv::Request>(Request);

        World = Owner->GetWorld();

        FString MyName = MoveRequest->GetName();
        FVector Location = MoveRequest->GetPose().GetPosition().GetVector();
        FQuat Quat = MoveRequest->GetPose().GetOrientation().GetQuat();
        bool ServiceSuccess = true;
        if(!World)
          {
            UE_LOG(LogTemp, Error, TEXT("World not found"));
          }
        else
          {
            // Execute on game thread
            FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
            {
              TActorIterator< AStaticMeshActor > ActorItr =
              TActorIterator< AStaticMeshActor >(World);
              while(ActorItr)
                {
                  if(ActorItr->GetName().Equals(MyName))
                    {
                      ActorItr->SetActorLocationAndRotation(Location, Quat);
                    }
                  ++ActorItr;
                }
            }, TStatId(), nullptr, ENamedThreads::GameThread);

            //wait code above to complete
            FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
          }

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FSpawnRobotGhostSrv::Response(ServiceSuccess));
}

void USpawnRobotGhostService::SetOwner(UObject* InOwner)
{
  Owner = Cast<URControllerComponent>(InOwner);
  if(!Owner)
    {
      UE_LOG(LogTemp, Error, TEXT("Owner of SimulationCommands no ControllerComponent"));
    }
}

void USpawnRobotGhostService::CreateServiceServer()
{
	ServiceServer =	MakeShareable<FSpawnRobotGhostServer>(new FSpawnRobotGhostServer(Name, Type, Owner));
}

void USpawnRobotGhostService::SetType()
{
	Type = TEXT("urobosim_msgs/SpawnRobotGhost");
}
