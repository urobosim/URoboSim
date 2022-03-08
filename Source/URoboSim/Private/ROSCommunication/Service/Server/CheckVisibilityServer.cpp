#include "ROSCommunication/Service/Server/CheckVisibilityServer.h"
#include "Controller/RController.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "srv/CheckVisibility.h"

DEFINE_LOG_CATEGORY_STATIC(LogRCheckVisibilityServer, Log, All)

URCheckVisibilityServer::URCheckVisibilityServer()
{
  Type = TEXT("urobosim_msgs/CheckVisibility");
}

void URCheckVisibilityServer::CreateServiceServer()
{
  ServiceServer = MakeShareable<FRCheckVisibilityServerCallback>(new FRCheckVisibilityServerCallback(Name, Type, GetWorld(), GetOwner()));
}

FRCheckVisibilityServerCallback::FRCheckVisibilityServerCallback(FString Name, FString Type, UWorld *InWorld, UObject *InOwner) : FROSBridgeSrvServer(Name, Type)
{
  World = InWorld;
  Owner = Cast<ARModel>(InOwner);
  if (Owner)
  {
    URCameraController *CameraController = Cast<URCameraController>(Owner->GetController("CameraController"));
    if (CameraController)
    {
      ARGBDCamera *Camera = CameraController->Camera;
      if (Camera)
      {
        PlayerIndex = Camera->GetAutoActivatePlayerIndex();
      }
    }
  }
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FRCheckVisibilityServerCallback::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<urobosim_msgs::CheckVisibilitySrv::Request> Request =
      MakeShareable(new urobosim_msgs::CheckVisibilitySrv::Request());
  Request->FromJson(JsonObject);
  return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FRCheckVisibilityServerCallback::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
  TSharedPtr<urobosim_msgs::CheckVisibilitySrv::Request> CheckVisibilityRequest =
      StaticCastSharedPtr<urobosim_msgs::CheckVisibilitySrv::Request>(Request);

  FString ObjectName = CheckVisibilityRequest->GetObjectName();
  bool ServiceSuccess = false;
  if (!World)
  {
    UE_LOG(LogRCheckVisibilityServer, Error, TEXT("World not found"));
  }
  else
  {
    // Execute on game thread
    FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]() {
      TActorIterator<AStaticMeshActor> ActorItr =
          TActorIterator<AStaticMeshActor>(World);
      while (ActorItr)
      {
        if (ActorItr->GetName().Equals(ObjectName))
        {
          APlayerController *PlayerController = UGameplayStatics::GetPlayerController(World, PlayerIndex);
          FVector2D ScreenPosition;
          ActorItr->GetStaticMeshComponent()->SetCastShadow(false);
          bool ProjectionSuccess = PlayerController->ProjectWorldLocationToScreen(ActorItr->GetActorLocation(), ScreenPosition);
          bool RenderSuccess = ActorItr->WasRecentlyRendered(0.1);
          ActorItr->GetStaticMeshComponent()->SetCastShadow(true);
          if (RenderSuccess && ProjectionSuccess)
          {
            ServiceSuccess = true;
          }
          break;
        }
        ++ActorItr;
      }
    },
                                                                         TStatId(), nullptr, ENamedThreads::GameThread);

    //wait code above to complete
    FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
  }

  return MakeShareable<FROSBridgeSrv::SrvResponse>(new urobosim_msgs::CheckVisibilitySrv::Response(ServiceSuccess));
}
