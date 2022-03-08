#include "ROSCommunication/Service/Server/MoveObjectActor.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "world_control_msgs/srv/MoveObject.h"

DEFINE_LOG_CATEGORY_STATIC(LogAMoveObjectActor, Log, All)

void AMoveObjectActor::BeginPlay()
{
  Super::BeginPlay();
  Handler = MakeShareable<FROSBridgeHandler>(new FROSBridgeHandler(WebsocketIPAddr, WebsocketPort));
  Handler->Connect();
  Server = MakeShareable<FRMoveObjectServerCallback>(new FRMoveObjectServerCallback(TEXT("unreal"), TEXT("move_object"), GetWorld(), this));
  Handler->AddServiceServer(Server);
}

void AMoveObjectActor::TickActor(float DeltaTime, ELevelTick TickType, FActorTickFunction &ThisTickFunction)
{
  Handler->Process();
}

void AMoveObjectActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Handler->Disconnect();
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FRMoveObjectServerCallback::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<FROSMoveObjectSrv::Request> Request =
      MakeShareable(new FROSMoveObjectSrv::Request());
  Request->FromJson(JsonObject);
  return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FRMoveObjectServerCallback::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
  TSharedPtr<FROSMoveObjectSrv::Request> MoveRequest =
      StaticCastSharedPtr<FROSMoveObjectSrv::Request>(Request);

  FString MyName = MoveRequest->GetName();
  FVector Location = MoveRequest->GetPose().GetPosition().GetVector();
  FQuat Quat = MoveRequest->GetPose().GetOrientation().GetQuat();
  bool ServiceSuccess = true;
  if (!World)
  {
    UE_LOG(LogAMoveObjectActor, Error, TEXT("World not found"));
  }
  else
  {
    // Execute on game thread
    FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]() {
      TActorIterator<AStaticMeshActor> ActorItr =
          TActorIterator<AStaticMeshActor>(World);
      while (ActorItr)
      {
        UE_LOG(LogAMoveObjectActor, Error, TEXT("Name %s"), *ActorItr->GetName());
        if (ActorItr->GetName().Equals(MyName))
        {
          ActorItr->SetActorLocationAndRotation(Location, Quat);
        }
        ++ActorItr;
      }
    },
                                                                         TStatId(), nullptr, ENamedThreads::GameThread);

    //wait code above to complete
    FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
  }

  return MakeShareable<FROSBridgeSrv::SrvResponse>(new FROSMoveObjectSrv::Response(ServiceSuccess));
}