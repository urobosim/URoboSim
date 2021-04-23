// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "ROSCommunication/CheckVisibilityServiceServer.h"
#include "Conversions.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSCheckVisebilityServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<urobosim_msgs::CheckVisibilitySrv::Request> Request =
          MakeShareable(new urobosim_msgs::CheckVisibilitySrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSCheckVisebilityServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
  TSharedPtr<urobosim_msgs::CheckVisibilitySrv::Request> CheckVisibilityRequest =
    StaticCastSharedPtr<urobosim_msgs::CheckVisibilitySrv::Request>(Request);


        FString ObjectName = CheckVisibilityRequest->GetObjectName();
        bool ServiceSuccess = false;
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
                  if(ActorItr->GetName().Equals(ObjectName))
                    {
                      APlayerController* PlayerController = UGameplayStatics::GetPlayerController(World, PlayerIndex);
                      FVector2D ScreenPosition;
                      ActorItr->GetStaticMeshComponent()->SetCastShadow(false);
                      bool ProjectionSuccess = PlayerController->ProjectWorldLocationToScreen(ActorItr->GetActorLocation(), ScreenPosition);
                      bool RenderSuccess = ActorItr->WasRecentlyRendered(0.1);
                      ActorItr->GetStaticMeshComponent()->SetCastShadow(true);
                      if(RenderSuccess && ProjectionSuccess)
                        {
                          ServiceSuccess = true;
                        }
                      break;
                    }
                  ++ActorItr;
                }
            }, TStatId(), nullptr, ENamedThreads::GameThread);

            //wait code above to complete
            FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
          }

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new urobosim_msgs::CheckVisibilitySrv::Response(ServiceSuccess));
}
