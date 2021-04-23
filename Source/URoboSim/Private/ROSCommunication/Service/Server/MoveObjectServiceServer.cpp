// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "ROSCommunication/MoveObjectServiceServer.h"
#include "Conversions.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSMoveObjectServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSMoveObjectSrv::Request> Request =
		MakeShareable(new FROSMoveObjectSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSMoveObjectServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
        TSharedPtr<FROSMoveObjectSrv::Request> MoveRequest =
		StaticCastSharedPtr<FROSMoveObjectSrv::Request>(Request);


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
                  UE_LOG(LogTemp, Error, TEXT("Name %s"), *ActorItr->GetName());
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
		(new FROSMoveObjectSrv::Response(ServiceSuccess));
}
