// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "ROSCommunication/ResetLevelServer.h"
#include "Conversions.h"
#include "GameFramework/GameMode.h"
// #include "EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSResetLevelServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSResetLevelSrv::Request> Request =
		MakeShareable(new FROSResetLevelSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSResetLevelServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
        bool ServiceSuccess = false;
        World = Owner->GetWorld();
        if(!World)
          {
              UE_LOG(LogTemp, Error, TEXT("No World"));
          }
        else
          {
            FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
            {
              AGameMode* GameMode = Cast<AGameMode>(UGameplayStatics::GetGameMode(World));

              if(GameMode)
                {
                  GameMode->RestartGame();
                }
              else
                {
                  UE_LOG(LogTemp, Error, TEXT("GameMode can not be cast to type AGameMode"));
                }
            }, TStatId(), nullptr, ENamedThreads::GameThread);
	FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
            ServiceSuccess = true;
          }

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSResetLevelSrv::Response(ServiceSuccess));
}
