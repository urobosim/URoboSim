// Copyright 2017-2019, Institute for Artificial Intelligence - University of Bremen

#include "ROSCommunication/CheckVisibilityServiceServer.h"
#include "Conversions.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSCheckVisebilityServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<urobosim_msgs::CheckVisibility::Request> Request =
          MakeShareable(new urobosim_msgs::CheckVisibility::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSCheckVisebilityServer::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
  TSharedPtr<urobosim_msgs::CheckVisibility::Request> CheckVisibilityRequest =
    StaticCastSharedPtr<urobosim_msgs::CheckVisibility::Request>(Request);


        FString ObjectName = CheckVisibilityRequest->GetObjectName();
        FVector BasePosition = CheckVisibilityRequest->GetBasePose().GetPosition().GetVector();
        FQuat BaseOrientation = CheckVisibilityRequest->GetBasePose().GetOrientation().GetQuat();
        FVector LookAtPosition = CheckVisibilityRequest->GetLookAtPosition().GetVector();
        // TArray<FString> Names = CheckVisibilityRequest->GetRobotPoses().GetJointNames();



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
                  if(ActorItr->GetName().Equals(ObjectName))
                    {
                      //Move robot to target position
                      BaseController->SetLocationAndRotation(FConversions::ROSToU(BasePosition),
                                                     FConversions::ROSToU(BaseOrientation).Rotator());
                      //Move robot into target configuration

                      //Look at target location


                    }
                  ++ActorItr;
                }
            }, TStatId(), nullptr, ENamedThreads::GameThread);

            //wait code above to complete
            FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);

            //Was target rendered
            // FTimerHandle MyTimerHandle;
            // Owner->GetWorldTimerManager().SetTimer(MyTimerHandle, JointController, &URJointController::FollowTrajectory, ActionTimeDiff, false);

          }

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new urobosim_msgs::CheckVisibility::Response(ServiceSuccess));
}
