#include "ROSCommunication/Service/Server/UpdateGraspPose.h"
#include "Controller/RController.h"
#include "EngineUtils.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "world_control_msgs/srv/SetModelPose.h"
#include "Tags.h"

DEFINE_LOG_CATEGORY_STATIC(LogRUpdateGraspPoseServer, Log, All)

URUpdateGraspPoseServer::URUpdateGraspPoseServer()
{
  Type = TEXT("world_control_msgs/SetModelPose");
}

void URUpdateGraspPoseServer::CreateServiceServer()
{
  ServiceServer = MakeShareable<FRUpdateGraspPoseServerCallback>(new FRUpdateGraspPoseServerCallback(Name, Type, GetWorld(), GetOwner()));
}

FRUpdateGraspPoseServerCallback::FRUpdateGraspPoseServerCallback(FString Name, FString Type, UWorld *InWorld, UObject *InOwner) : FROSBridgeSrvServer(Name, Type)
{
  World = InWorld;
  Owner = Cast<ARModel>(InOwner);
}

TSharedPtr<FROSBridgeSrv::SrvRequest> FRUpdateGraspPoseServerCallback::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
  TSharedPtr<FROSSetModelPoseSrv::Request> Request =
      MakeShareable(new FROSSetModelPoseSrv::Request());
  Request->FromJson(JsonObject);
  return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FRUpdateGraspPoseServerCallback::Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
  TSharedPtr<FROSSetModelPoseSrv::Request> UpdateGraspPoseRequest =
      StaticCastSharedPtr<FROSSetModelPoseSrv::Request>(Request);

  FString UniqueId = UpdateGraspPoseRequest->GetId();
  FVector Location = FConversions::ROSToU(UpdateGraspPoseRequest->GetPose().GetPosition().GetVector());
  FRotator Rotator = FRotator(FConversions::ROSToU(UpdateGraspPoseRequest->GetPose().GetOrientation().GetQuat()));

  TArray<FString> GripperControllerList;
  GripperControllerList.Add(TEXT("RGripperController"));
  GripperControllerList.Add(TEXT("R1GripperController"));
  GripperControllerList.Add(TEXT("R2GripperController"));
  GripperControllerList.Add(TEXT("R3GripperController"));
  GripperControllerList.Add(TEXT("R4GripperController"));
  GripperControllerList.Add(TEXT("LGripperController"));
  GripperControllerList.Add(TEXT("L1GripperController"));
  GripperControllerList.Add(TEXT("L2GripperController"));
  GripperControllerList.Add(TEXT("L3GripperController"));
  GripperControllerList.Add(TEXT("L4GripperController"));
  TArray<FString> ActiveControllerList;
  bool bActorAttached = false;
  bool ServiceSuccess = false;

  if (Owner)
  {
      if (!World)
      {
        UE_LOG(LogRUpdateGraspPoseServer, Error, TEXT("World not found"));
      }
      else
      {
        // Execute on game thread
        FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]() {

                World = GEngine->GameViewport->GetWorld();
                TArray<AActor*> AllMatchingActors = FTags::GetActorsWithKeyValuePair(World, TEXT("SemLog"), TEXT("Id"), UniqueId);

		AActor* Actor = nullptr;
		if(AllMatchingActors.Num() > 0)
			Actor = AllMatchingActors.Pop();

		if (!Actor)
		{
			// Couldn't find Actor for ID
			UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" does not exist and can therefore not be moved."), *UniqueId);
			ServiceSuccess = false;
			return;
		}

                for(auto& GC: GripperControllerList)
                  {
                    URGripperControllerBase *GripperController = Cast<URGripperControllerBase>(Owner->GetController(GC));
                    if (GripperController)
                      {
                        AStaticMeshActor* FixatedObject = GripperController->GraspComponent->FixatedObject;
                        if(FixatedObject)
                          {
                            if(FixatedObject == Actor)
                              {
                                UPhysicsConstraintComponent* Constraint = GripperController->GraspComponent->Constraint;
                                bActorAttached = true;
                                // Constraint->TermComponentConstraint();

                                FixatedObject->SetActorLocationAndRotation(Location, Rotator, false, NULL, ETeleportType::TeleportPhysics);
                                Constraint->InitComponentConstraint();
                              }
                          }
                      }
                  }
                if(!bActorAttached)
                  {
                    UE_LOG(LogTemp, Warning, TEXT("Actor with id:\"%s\" is not grasped and can therefore not be moved."), *UniqueId);
                  }


        }, TStatId(), nullptr, ENamedThreads::GameThread);

        //wait code above to complete
        FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
      }

  }
	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSSetModelPoseSrv::Response(ServiceSuccess));
}
