// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "ROSCommunication/ResetLevelServer.h"
#include "Conversions.h"
#include "EngineUtils.h"
#include "ROSCommunication/RWorldControl.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/GameFramework/SpectatorPawn.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"






TSharedPtr<FROSBridgeSrv::SrvRequest> FROSResetLevelServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSResetLevelSrv::Request> Request =
		MakeShareable(new FROSResetLevelSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSResetLevelServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSResetLevelSrv::Request> SpawnMeshRequest =
		StaticCastSharedPtr<FROSResetLevelSrv::Request>(Request);


        bool ServiceSuccess = false;
        World = GEditor->GetPIEWorldContext()->World();
        if(!World)
          {
              UE_LOG(LogTemp, Error, TEXT("No World"));
          }
        else
          {
            // Execute on game thread
            FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
            {
              APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, 0);
              UE_LOG(LogTemp, Error, TEXT("Begin Reset"));
              ASpectatorPawn * Spectator = PlayerController->GetSpectatorPawn();
              FTransform Pose = Spectator->GetActorTransform();
              TActorIterator< AWorldController > ActorItr =
              TActorIterator< AWorldController >(World);

              while(ActorItr)
                {
                  UE_LOG(LogTemp, Error, TEXT("Actor Name %s"), *ActorItr->GetName());
                  if(ActorItr->GetName().Contains(TEXT("WorldController")))
                    {
                      UE_LOG(LogTemp, Error, TEXT("ActorFound"));
                      // ActorItr->SetActorLocationAndRotation(Location, Quat);
                      ActorItr->ResetLevel();
                    }
                  ++ActorItr;
                }

              // World = GEditor->GetPIEWorldContext()->World();
              // PlayerController = UGameplayStatics::GetPlayerController(World, 0);
              // Spectator = PlayerController->GetSpectatorPawn();
              Spectator->SetActorTransform(Pose);


            }, TStatId(), nullptr, ENamedThreads::GameThread);

            //wait code above to complete
            FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);




            ServiceSuccess = true;
          }

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSResetLevelSrv::Response(ServiceSuccess));
}
